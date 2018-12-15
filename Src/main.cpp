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

#include "cmsis_os.h"

#include "gfx.h"
#include "can_gui_package.h"
#include "event_listener.h"
#include "edit_can_message.h"
#include "can_view.h"
#include "rx_can_view.h"
#include "tx_can_view.h"
#include "add_can_message.h"
#include "main_view.h"
#include <stdio.h>

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
CAN_HandleTypeDef hcan;

SD_HandleTypeDef hsd1;
gfxQueueGSync *canNotificationQueue;
/* USER CODE BEGIN PV */
extern "C" void __attribute ((weak)) _init(void) {
};
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

static void MX_GPIO_Init(void);

static void MX_CAN1_Init(void);
static void MX_SDMMC1_SD_Init(void);

void StartDefaultTask(void const *argument);

void transmitThread(void *_);

void receiveThread(void *_);

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
    MX_CAN1_Init();
    MX_SDMMC1_SD_Init();
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

    canNotificationQueue = (gfxQueueGSync *) gfxAlloc(sizeof(gfxQueueGSync));
    canTransmitQueue = (gfxQueueGSync *) gfxAlloc(sizeof(gfxQueueGSync));
    canReceiveQueue = (gfxQueueGSync *) gfxAlloc(sizeof(gfxQueueGSync));
    gfxQueueGSyncInit(canNotificationQueue);
    gfxQueueGSyncInit(canTransmitQueue);
    gfxQueueGSyncInit(canReceiveQueue);

    osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 3, 1024);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    gfxThreadCreate(NULL, 128, NORMAL_PRIORITY, transmitThread, 0);
    gfxThreadCreate(NULL, 128, NORMAL_PRIORITY, receiveThread, 0);
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
#define SD_DETECT_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOC_CLK_ENABLE()
#define SD_DETECT_GPIO_CLK_DISABLE()         __HAL_RCC_GPIOC_CLK_DISABLE()
#define SD_DETECT_EXTI_IRQn                  EXTI15_10_IRQn
static void MX_SDMMC1_SD_Init(void)
{
//	BSP_SD_Init();
  /* USER CODE BEGIN SDMMC1_Init 0 */

  /* USER CODE END SDMMC1_Init 0 */

  /* USER CODE BEGIN SDMMC1_Init 1 */

  /* USER CODE END SDMMC1_Init 1 */
//    uint8_t sd_state = MSD_OK;
//
//    /* uSD device interface configuration */
//    hsd1.Instance = SDMMC1;
//
//    hsd1.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
//    hsd1.Init.ClockBypass         = SDMMC_CLOCK_BYPASS_DISABLE;
//    hsd1.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
//    hsd1.Init.BusWide             = SDMMC_BUS_WIDE_1B;
//    hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
//    hsd1.Init.ClockDiv            = SDMMC_TRANSFER_CLK_DIV;
//
//    /* Msp SD Detect pin initialization */
//
//    GPIO_InitTypeDef  gpio_init_structure;
//
//    SD_DETECT_GPIO_CLK_ENABLE();
//
//    /* GPIO configuration in input for uSD_Detect signal */
//    gpio_init_structure.Pin       = SD_DETECT_PIN;
//    gpio_init_structure.Mode      = GPIO_MODE_INPUT;
//    gpio_init_structure.Pull      = GPIO_PULLUP;
//    gpio_init_structure.Speed     = GPIO_SPEED_HIGH;
//    HAL_GPIO_Init(SD_DETECT_GPIO_PORT, &gpio_init_structure);
////    if(BSP_SD_IsDetected() != SD_PRESENT)   /* Check if SD card is present */
////    {
////        return MSD_ERROR_SD_NOT_PRESENT;
////    }
//
//    /* Msp SD initialization */
////    BSP_SD_MspInit(&hsd1, NULL);
//
//    /* HAL SD initialization */
//    if(HAL_SD_Init(&hsd1) != HAL_OK)
//    {
//        sd_state = MSD_ERROR;
//    }
//
//    /* Configure SD Bus width */
//    if(sd_state == MSD_OK)
//    {
//        /* Enable wide operation */
//        if(HAL_SD_ConfigWideBusOperation(&hsd1, SDMMC_BUS_WIDE_4B) != HAL_OK)
//        {
//            sd_state = MSD_ERROR;
//        }
//        else
//        {
//            sd_state = MSD_OK;
//        }
//    }
//
////    return  sd_state;
  //HAL_SD_Init(&hsd1);
  /* USER CODE BEGIN SDMMC1_Init 2 */

  /* USER CODE END SDMMC1_Init 2 */

}
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

/* CAN1 init function */
static void MX_CAN1_Init(void) {

    hcan.Instance = CAN1;
    hcan.Init.Prescaler = 50;
    hcan.Init.Mode = CAN_MODE_NORMAL;
    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1 = CAN_BS1_7TQ;
    hcan.Init.TimeSeg2 = CAN_BS2_2TQ;
    hcan.Init.TimeTriggeredMode = DISABLE;
    hcan.Init.AutoBusOff = DISABLE;
    hcan.Init.AutoWakeUp = DISABLE;
    hcan.Init.AutoRetransmission = DISABLE;
    hcan.Init.ReceiveFifoLocked = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;
//    if (HAL_CAN_Init(&hcan) != HAL_OK) {
//        _Error_Handler(__FILE__, __LINE__);
//    }

    CAN_FilterTypeDef sFilterConfig;
    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

//    if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK) {
//        _Error_Handler(__FILE__, __LINE__);
//
//    }
//    /* rx fifo0 message pending aktivieren*/
//    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
//        /* Notification Error */
//        _Error_Handler(__FILE__, __LINE__);
//    }
//    /* rx fifo1 message pending aktivieren*/
//    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK) {
//        /* Notification Error */
//        _Error_Handler(__FILE__, __LINE__);
//    }
//    if (HAL_CAN_Start(&hcan) != HAL_OK) {
//        _Error_Handler(__FILE__, __LINE__);
//    }

}

CAN_RxHeaderTypeDef RxHeader;
uint8_t RxData[8];

void receiveThread(void *_) {
    while (1) {
        gfxQueueGSyncGet(canNotificationQueue, TIME_INFINITE);
        if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
            _Error_Handler(__FILE__, __LINE__);
        }
        can_gui_package* rxPackage = new can_gui_package();
        rxPackage->id = RxHeader.StdId;
        rxPackage->isRemote = RxHeader.RTR;
        rxPackage->dlc = RxHeader.DLC;
        for (int i = 0; i < RxHeader.DLC; i++) {
            rxPackage->data.data_b[i] = RxData[i];
        }
//        gfxQueueGSyncPut(canReceiveQueue, &rxPackage->q_item);
        cMainView.addRxCanPackage(rxPackage);
        if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
            /* Notification Error */
            _Error_Handler(__FILE__, __LINE__);
        }
        HAL_Delay(100);
    }
}

gfxQueueGSyncItem q_item;

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (HAL_CAN_DeactivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        /* Notification Error */
        _Error_Handler(__FILE__, __LINE__);
        return;
    }
    gfxQueueGSyncPushI(canNotificationQueue, &q_item);
//    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
//        _Error_Handler(__FILE__, __LINE__);
//    }
//    rxPackage->id = RxHeader.StdId;
//    rxPackage->isRemote = RxHeader.RTR;
//    rxPackage->dlc = RxHeader.DLC;
//    for (int i = 0; i < RxHeader.DLC; i++) {
//    	rxPackage->data.data_b[i] = RxData[i];
//    }
//    gfxQueueGSyncPushI(canReceiveQueue, &rxPackage->q_item);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    /*CAN_RxHeaderTypeDef   RxHeader;
    uint8_t               RxData[8];
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, RxData) != HAL_OK) {
        _Error_Handler(__FILE__, __LINE__);
    }
	can_gui_package* package = new can_gui_package();

    package->id = RxHeader.StdId;
    package->isRemote = RxHeader.RTR;
    package->dlc = RxHeader.DLC;
    for (int i = 0; i < 8; i++) {
        package->data.data_b[i] = RxData[i];
    }
    gfxQueueGSyncPush(canTransmitQueue, &package->q_item);*/
}

static void MX_GPIO_Init(void) {


}

/* USER CODE BEGIN 4 */
CAN_TxHeaderTypeDef TxHeader;
uint8_t TxData[8];
uint32_t TxMailbox;

void transmitThread(void *_) {
    while (true) {
        gfxQueueGSyncItem *package_queued = gfxQueueGSyncGet(canTransmitQueue, TIME_INFINITE);
        auto package = (can_gui_package *) package_queued;
        TxHeader.StdId = package->id;
        TxHeader.ExtId = 0x00;
        TxHeader.RTR = package->isRemote;
        TxHeader.IDE = CAN_ID_STD;
        TxHeader.DLC = package->dlc;
        TxHeader.TransmitGlobalTime = DISABLE;
        for (int i = 0; i < 8; i++) {
            TxData[i] = package->data.data_b[i];
        }
        if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
            continue;
        }
    }
}
/* USER CODE END 4 */

/* StartDefaultTask function */
// extern void initMainPage(void);

void StartDefaultTask(void const *argument) {

//	if (gfileMount('F', "/"))
//		gfxHalt("Can't mount the FAT file system");
//    GFILE* file = gfileOpen("asd", "wx");
//	if(!file) {
//		gfxHalt("Can't open the file file.txt");;
//	}
//
//    const char *buf = "deine mutter";
//    volatile size_t len = gfileWrite(file, buf, 12);
//    gfileClose(file);
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
