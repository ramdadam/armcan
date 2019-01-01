/**
 ******************************************************************************
 * File Name          : main.c
 * Description        : Main program body
 ******************************************************************************
 * This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * Copyright (c) 2017 STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gfx.h"
#include "can_gui_package.h"
#include "can_driver.h"
#include "cmsis_os.h"

#include "event_listener.h"
#include "sd_settings_view.h"
#include "Inc/View/edit_can_message.h"
#include "Inc/View/chat_view.h"
#include "can_view.h"
#include "Inc/View/rx_can_view.h"
#include "Inc/View/tx_can_view.h"
#include "Inc/View/add_can_message.h"
#include "Inc/View/can_settings_view.h"

#include "Inc/View/main_view.h"

#include <stdio.h>
#include <Inc/fatfs.h>

/* USER CODE BEGIN Includes */
extern gfxQueueGSync *canTransmitQueue;
extern gfxQueueGSync *canReceiveQueue;


void *operator new(size_t size) {
    return gfxAlloc(size);
}

void operator delete(void *ptr, size_t size) {
    gfxFree(ptr);
}
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/


osThreadId defaultTaskHandle;

SD_HandleTypeDef hsd1;
gfxQueueGSync *canNotificationQueue;
gfxQueueGSync *txMailboxNotificationQueue;
gfxQueueGSync *sdNotificationQueue;
/* USER CODE BEGIN PV */
extern "C" void __attribute ((weak)) _init(void) {
};
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPIO_Init(void);


void StartDefaultTask(void const *argument);

void transmitThread(void *_);

void receiveThread(void *_);
void sdNotificationThread(void *_);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
CMainView cMainView;

int main(void) {

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration----------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* Create the thread(s) */
    /* definition and creation of defaultTask */

    /* USER CODE BEGIN RTOS_THREADS */

    sdNotificationQueue = (gfxQueueGSync *) gfxAlloc(sizeof(gfxQueueGSync));
    canNotificationQueue = (gfxQueueGSync *) gfxAlloc(sizeof(gfxQueueGSync));
    canTransmitQueue = (gfxQueueGSync *) gfxAlloc(sizeof(gfxQueueGSync));
    canReceiveQueue = (gfxQueueGSync *) gfxAlloc(sizeof(gfxQueueGSync));
    gfxQueueGSyncInit(sdNotificationQueue);
    gfxQueueGSyncInit(canNotificationQueue);
    gfxQueueGSyncInit(canTransmitQueue);
    gfxQueueGSyncInit(canReceiveQueue);
    gfxQueueGSyncInit(txMailboxNotificationQueue);

    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 3, 1024);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

//    gfxThreadCreate(NULL, 128, LOW_PRIORITY, transmitThread, 0);
    gfxThreadCreate(NULL, 128, LOW_PRIORITY + 1, receiveThread, 0);
    gfxThreadCreate(NULL, 128, LOW_PRIORITY, sdNotificationThread, 0);

    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */

    }
    /* USER CODE END 3 */

}
#define SD_DETECT_PIN                        GPIO_PIN_13
#define SD_DETECT_GPIO_PORT                  GPIOC
#define SD_DETECT_EXTI_IRQn                  EXTI15_10_IRQn

/** System Clock Configuration
 */
void SystemClock_Config(void) {

	  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	  /**Configure the main internal regulator output voltage
	  */
	  __HAL_RCC_PWR_CLK_ENABLE();
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	  /**Initializes the CPU, AHB and APB busses clocks
	  */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	  RCC_OscInitStruct.PLL.PLLM = 16;
	  RCC_OscInitStruct.PLL.PLLN = 400;
	  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	  RCC_OscInitStruct.PLL.PLLQ = 8;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  /**Activate the Over-Drive mode
	  */
	  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
	  {
	    Error_Handler();
	  }
	  /**Initializes the CPU, AHB and APB busses clocks
	  */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
	  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
	  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
	  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void receiveThread(void *_) {
    while (true) {
        gfxQueueGSyncGet(canNotificationQueue, TIME_INFINITE);
        auto rxPackage = canDriver.receiveCANPackage();
        cMainView.addRxCanPackage(rxPackage);
    }
}
void sdNotificationThread(void *_) {
    while (true) {
        gfxQueueGSyncGet(sdNotificationQueue, TIME_INFINITE);
        cMainView.notifySdCardChanges();
    }
}

gfxQueueGSyncItem q_item;
gfxQueueGSyncItem q_item2;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        /* Notification Error */
        _Error_Handler(__FILE__, __LINE__);
        return;
    }
    gfxQueueGSyncPushI(canNotificationQueue, &q_item);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK) {
        /* Notification Error */
        _Error_Handler(__FILE__, __LINE__);
        return;
    }
    gfxQueueGSyncPushI(canNotificationQueue, &q_item2);
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan) {
    canDriver.clearMailboxPendingTx0();
}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan) {
    canDriver.clearMailboxPendingTx1();
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan) {
    canDriver.clearMailboxPendingTx2();
}

static void MX_GPIO_Init(void) {


}

/* USER CODE BEGIN 4 */
//CAN_TxHeaderTypeDef TxHeader;
//uint8_t TxData[8];
//uint32_t TxMailbox;
bool firstRun = true;
void transmitThread(void *_) {
    while (true) {
//        gfxQueueGSyncItem *package_queued = gfxQueueGSyncGet(canTransmitQueue, TIME_INFINITE);
//        if(firstRun) {
//        	firstRun = false;
//        	continue;
//        }
//        auto package = (can_gui_package *) package_queued;
//        TxHeader.StdId = package->id;
//        TxHeader.ExtId = 0x00;
//        TxHeader.RTR = package->isRemote;
//        TxHeader.IDE = CAN_ID_STD;
//        TxHeader.DLC = package->dlc;
//        TxHeader.TransmitGlobalTime = DISABLE;
//        for (int i = 0; i < 8; i++) {
//            TxData[i] = package->data.data_b[i];
//        }
//        if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
//            continue;
//        }
    }
}
/* USER CODE END 4 */

/* StartDefaultTask function */
// extern void initMainPage(void);
void StartDefaultTask(void const *argument) {
    cMainView.initMainPage();
}
/* USER CODE END 5 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void _Error_Handler(char *file, int line) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */

}

#endif

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
