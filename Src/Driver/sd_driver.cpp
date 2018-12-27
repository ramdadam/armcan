//
// Created by mohamad on 28.12.18.
//

#include <Inc/Driver/sd_driver.h>

#include "stm32f7xx_hal_sd.h"
#include <Inc/fatfs.h>
#include "gfx.h"
#include <stdio.h>
#include <string.h>
#include "bsp_driver_sd.h"
#include "sd_driver.h"

extern SD_HandleTypeDef hsd1;
extern gfxQueueGSync *sdNotificationQueue;

gfxQueueGSyncItem q_item7;

CSdDriver::CSdDriver() {
    BSP_SD_ITConfig();
}

void SD_Driver_IRQ::EXTI15_10_IRQHandler(void) {
    HAL_GPIO_EXTI_IRQHandler(SD_DETECT_PIN);
    sdDriver.sdDetected = sdDriver.isSdConnected();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == SD_DETECT_PIN) {
        gfxQueueGSyncPushI(sdNotificationQueue, &q_item7);
    }
}

bool CSdDriver::initSdDriver() {
    bool isLinked = FATFS_LinkDriver(&SD_Driver, SDPath) == FR_OK;

    sdDetected = isSdConnected();

    return sdDetected && !isLinked;
}

bool CSdDriver::isSdConnected() {
    uint8_t status = SD_PRESENT;

    /* Check SD card detect pin */
    if (HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT, SD_DETECT_PIN) == GPIO_PIN_SET) {
        status = SD_NOT_PRESENT;
    }
    return status;
}

uint32_t CSdDriver::getSDFreeSpace() {
    return getFreeSpace();
}

uint32_t CSdDriver::getSDTotalSpace() {
    return getTotalSpace();
}

uint32_t CSdDriver::getSDState(char *text, uint32_t *sdStateCode) {
    sdState = HAL_SD_GetState(&hsd1);
    *sdStateCode = sdState;
    switch (sdState) {
        case HAL_SD_STATE_RESET: {
            const char *SD_STATE_TEXT = "SD not yet initialized or disabled";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 1;
        }
        case HAL_SD_STATE_READY: {
            const char *SD_STATE_TEXT = "SD initialized and ready for use";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 0;
        }
        case HAL_SD_STATE_TIMEOUT: {
            const char *SD_STATE_TEXT = "SD Timeout state";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 1;
        }
        case HAL_SD_STATE_BUSY: {
            const char *SD_STATE_TEXT = "SD process ongoing";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 0;
        }
        case HAL_SD_STATE_PROGRAMMING: {
            const char *SD_STATE_TEXT = "SD Programming State";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 0;
        }
        case HAL_SD_STATE_RECEIVING: {
            const char *SD_STATE_TEXT = "SD Receiving State";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 0;
        }
        case HAL_SD_STATE_TRANSFER: {
            const char *SD_STATE_TEXT = "SD Transfer State";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 0;
        }
        case HAL_SD_STATE_ERROR: {
            const char *SD_STATE_TEXT = "SD is in error state";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 1;
        }
    }
}

uint32_t CSdDriver::getSDCardState(char *text, uint32_t *sdCardStateCode) {
    sdCardState = HAL_SD_GetCardState(&hsd1);

    switch (sdCardState) {
        case HAL_SD_CARD_READY: {
            const char *SD_STATE_TEXT = "Card state is ready";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 0;
        }
        case HAL_SD_CARD_IDENTIFICATION: {
            const char *SD_STATE_TEXT = "Card is in identification state";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 0;
        }
        case HAL_SD_CARD_STANDBY: {
            const char *SD_STATE_TEXT = "Card is in standby state";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 1;
        }
        case HAL_SD_CARD_TRANSFER: {
            const char *SD_STATE_TEXT = "Card is in transfer state";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 0;
        }
        case HAL_SD_CARD_SENDING: {
            const char *SD_STATE_TEXT = "Card is sending an operation";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 0;
        }
        case HAL_SD_CARD_RECEIVING: {
            const char *SD_STATE_TEXT = "Card is receiving operation information";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 0;
        }
        case HAL_SD_CARD_PROGRAMMING: {
            const char *SD_STATE_TEXT = "Card is in programming state";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 0;
        }
        case HAL_SD_CARD_DISCONNECTED: {
            const char *SD_STATE_TEXT = "Card is disconnected";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 0;
        }
        case HAL_SD_CARD_ERROR: {
            const char *SD_STATE_TEXT = "Card response Error";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return 1;
        }
        default: {
            const char *SD_STATE_TEXT = !sdDetected ? "Insert SD Card" : "Waiting for SD Operation...";
            strncpy(text, SD_STATE_TEXT, strlen(SD_STATE_TEXT));
            return !sdDetected;
        }
    }
}

bool CSdDriver::formatFAT32() {
    return (f_mkfs((TCHAR const *) SDPath, FM_FAT32, 0, buffer, sizeof(buffer)) != FR_OK);
}

bool CSdDriver::mountSDCard() {
    return (f_mount(&SDFatFS, (TCHAR const *) SDPath, 0) != FR_OK);
}

bool CSdDriver::unmountSDCard() {
    return (f_mount(0, "", 0) != FR_OK);
}

bool CSdDriver::saveScreenshot() {
    char nameBuffer[30] = {" "};
    snprintf(nameBuffer, 30, "IMG_%d.pgm", imageCounter);
    if (f_open(&SDFile, nameBuffer, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK) {
        return false;
    }
    int16_t width = 480;
    int16_t height = 272;
    uint byteswritten;
    f_printf(&SDFile, "P6\n%d %d\n255\n", width, height);
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            uint32_t color = gdispGetPixelColor(x, y);
            static uint8_t colorRGB[3];
            colorRGB[0] = GREEN_OF(color);  /* green */
            colorRGB[1] = BLUE_OF(color);  /* blue */
            colorRGB[2] = RED_OF(color);  /* red */
            f_write(&SDFile, colorRGB, 3, &byteswritten);
        }
    }

    imageCounter += 1;
    f_close(&SDFile);
}

bool CSdDriver::deInitSdDriver() {
    return FATFS_UnLinkDriver(SDPath) != FR_OK;;
}

CSdDriver sdDriver;
