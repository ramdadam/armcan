//
// Created by mohamad on 20.12.18.
//
#include <stdio.h>
#include <string.h>
#include "gfx.h"
#include "can_gui_package.h"
#include "stm32f7xx_hal.h"

#include <Inc/Driver/can_driver.h>
CCanDriver canDriver;

void CCanDriver::MX_CAN1_Init(uint16_t prescaler = DEFAULT_CAN_PRESCALER, bool sleepMode = false) {

    hcan.Instance = CAN1;
    hcan.Init.Prescaler = prescaler;
    hcan.Init.Mode = sleepMode ? CAN_MODE_SILENT : CAN_MODE_NORMAL;
    hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
    hcan.Init.TimeSeg1 = CAN_BS1_8TQ;
    hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
    hcan.Init.TimeTriggeredMode = DISABLE;
    hcan.Init.AutoBusOff = DISABLE;
    hcan.Init.AutoWakeUp = DISABLE;
    hcan.Init.AutoRetransmission = DISABLE;
    hcan.Init.ReceiveFifoLocked = DISABLE;
    hcan.Init.TransmitFifoPriority = DISABLE;

    if (HAL_CAN_Init(&hcan) != HAL_OK) {
        return;
    }

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

    if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK) {
        return;
    }

    /* rx fifo0 message pending aktivieren*/
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        return;
    }

    /* rx fifo1 message pending aktivieren*/
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK) {
        return;
    }
    /* rx fifo1 message pending aktivieren*/
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK) {
        return;
    }

    if (HAL_CAN_Start(&hcan) != HAL_OK) {
        return;
    }

}

can_gui_package *CCanDriver::receiveCANPackage() {
    if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
        return nullptr;
    }
    can_gui_package * rxPackage = new can_gui_package();
    rxPackage->id = RxHeader.StdId;
    rxPackage->isRemote = RxHeader.RTR;
    rxPackage->dlc = RxHeader.DLC;
    for (int i = 0; i < RxHeader.DLC; i++) {
        rxPackage->data.data_b[i] = RxData[i];
    }
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
        /* Notification Error */
        return nullptr;
    }
    if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK) {
        /* Notification Error */
        return nullptr;

    }
    return rxPackage;
}

CCanDriver::CCanDriver() {

}

uint8_t CCanDriver::sendCANPackage(can_gui_package *package) {
    package->count += 1;
    bumpPackageCounter(package);

    uint8_t TxData0[8] = {0};
    uint8_t TxData1[8] = {0};
    uint8_t TxData2[8] = {0};
    TxHeader.StdId = package->id;
    TxHeader.ExtId = 0x00;
    TxHeader.RTR = package->isRemote;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = package->dlc;
    TxHeader.TransmitGlobalTime = DISABLE;

    volatile uint32_t freelevel = HAL_CAN_GetTxMailboxesFreeLevel(&hcan);
    if (freelevel > 0) {
        for (int i = 0; i < 8; i++) {
            TxData0[i] = package->data.data_b[i];
        }
        HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData0, &TxMailbox);
    }
}


uint8_t CCanDriver::getUserFriendlyErrorText(char *text, uint32_t* canErrorCode) {
    uint32_t currentCanErrorCode = HAL_CAN_GetError(&hcan);
    *canErrorCode = currentCanErrorCode;
    switch(currentCanErrorCode) {
        case HAL_CAN_ERROR_NONE: {
            const char *CAN_ERROR_TEXT = "No error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 0;
        }
        case HAL_CAN_ERROR_EWG: {
            const char *CAN_ERROR_TEXT = "Protocol Error Warning ";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_EPV: {
            const char *CAN_ERROR_TEXT = "Error Passive";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_BOF: {
            const char *CAN_ERROR_TEXT = "Bus-off error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_STF: {
            const char *CAN_ERROR_TEXT = "Stuff error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_FOR: {
            const char *CAN_ERROR_TEXT = "Form error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_ACK: {
            const char *CAN_ERROR_TEXT = "Acknowledgment error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_BR: {
            const char *CAN_ERROR_TEXT = "Bit recessive error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_BD: {
            const char *CAN_ERROR_TEXT = "Bit dominant error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_CRC: {
            const char *CAN_ERROR_TEXT = "CRC error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_RX_FOV0: {
            const char *CAN_ERROR_TEXT = "Rx FIFO0 overrun error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_RX_FOV1: {
            const char *CAN_ERROR_TEXT = "Rx FIFO1 overrun error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_TX_ALST0: {
            const char *CAN_ERROR_TEXT = "TxMailbox 0 transmit failure due to arbitration lost";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_TX_TERR0: {
            const char *CAN_ERROR_TEXT = "TxMailbox 0 transmit failure due to tranmit error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_TX_ALST1: {
            const char *CAN_ERROR_TEXT = "TxMailbox 1 transmit failure due to arbitration lost";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_TX_TERR1: {
            const char *CAN_ERROR_TEXT = "TxMailbox 1 transmit failure due to tranmit error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_TX_ALST2: {
            const char *CAN_ERROR_TEXT = "TxMailbox 2 transmit failure due to arbitration lost";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_TX_TERR2: {
            const char *CAN_ERROR_TEXT = "TxMailbox 2 transmit failure due to tranmit error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_TIMEOUT: {
            const char *CAN_ERROR_TEXT = "Timeout error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_NOT_INITIALIZED: {
            const char *CAN_ERROR_TEXT = "Peripheral not initialized";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_NOT_READY: {
            const char *CAN_ERROR_TEXT = "Peripheral not ready";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_NOT_STARTED: {
            const char *CAN_ERROR_TEXT = "Peripheral not started";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        case HAL_CAN_ERROR_PARAM: {
            const char *CAN_ERROR_TEXT = "Parameter error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
        default: {
            const char *CAN_ERROR_TEXT = "Unkown error";
            strncpy(text, CAN_ERROR_TEXT, strlen(CAN_ERROR_TEXT));
            return 1;
        }
    }
}

uint8_t CCanDriver::getUserFriendlyState(char *text, uint32_t* canState) {
    HAL_CAN_StateTypeDef state = HAL_CAN_GetState(&hcan);
    *canState = state;
    switch(state) {
        case HAL_CAN_STATE_RESET:
        {
            const char *CAN_STATE_TEXT = "CAN not yet initialized or disabled";
            strncpy(text, CAN_STATE_TEXT, strlen(CAN_STATE_TEXT));
            return 1;
        }
        case HAL_CAN_STATE_READY:
        {
            const char *CAN_STATE_TEXT = "CAN initialized and ready for use";
            strncpy(text, CAN_STATE_TEXT, strlen(CAN_STATE_TEXT));
            return 0;
        }
        case HAL_CAN_STATE_LISTENING:
        {
            const char *CAN_STATE_TEXT = "CAN receive process is ongoing";
            strncpy(text, CAN_STATE_TEXT, strlen(CAN_STATE_TEXT));
            return 0;
        }
        case HAL_CAN_STATE_SLEEP_PENDING:
        {
            const char *CAN_STATE_TEXT = "CAN sleep request is pending";
            strncpy(text, CAN_STATE_TEXT, strlen(CAN_STATE_TEXT));
            return 1;
        }
        case HAL_CAN_STATE_SLEEP_ACTIVE:
        {
            const char *CAN_STATE_TEXT = "CAN sleep mode is active";
            strncpy(text, CAN_STATE_TEXT, strlen(CAN_STATE_TEXT));
            return 0;
        }
        case HAL_CAN_STATE_ERROR:
        {
            const char *CAN_STATE_TEXT = "CAN error state";
            strncpy(text, CAN_STATE_TEXT, strlen(CAN_STATE_TEXT));
            return 1;
        }
        default: {
            const char *CAN_STATE_TEXT = "Unknown state";
            strncpy(text, CAN_STATE_TEXT, strlen(CAN_STATE_TEXT));
            return 1;
        }
    }
}

namespace CAN_driver_ISR // need a namespace to declare friend functions
{
/**
* @brief This function handles CAN1 TX interrupts.
*/
    extern "C" void CAN1_TX_IRQHandler(void) {
    /* USER CODE BEGIN CAN1_TX_IRQn 0 */

    /* USER CODE END CAN1_TX_IRQn 0 */
    HAL_CAN_IRQHandler(canDriver.getHandle());
    /* USER CODE BEGIN CAN1_TX_IRQn 1 */

    /* USER CODE END CAN1_TX_IRQn 1 */
}

/**
* @brief This function handles CAN1 RX0 interrupts.
*/
    extern "C" void CAN1_RX0_IRQHandler(void) {
    /* USER CODE BEGIN CAN1_RX0_IRQn 0 */

    /* USER CODE END CAN1_RX0_IRQn 0 */
    HAL_CAN_IRQHandler(canDriver.getHandle());
    /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

    /* USER CODE END CAN1_RX0_IRQn 1 */
}

/**
* @brief This function handles CAN1 RX1 interrupt.
*/
    extern "C" void CAN1_RX1_IRQHandler(void) {
    /* USER CODE BEGIN CAN1_RX1_IRQn 0 */

    /* USER CODE END CAN1_RX1_IRQn 0 */
    HAL_CAN_IRQHandler(canDriver.getHandle());
    /* USER CODE BEGIN CAN1_RX1_IRQn 1 */

    /* USER CODE END CAN1_RX1_IRQn 1 */
}

}
