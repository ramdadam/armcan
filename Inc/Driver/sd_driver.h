//
// Created by mohamad on 28.12.18.
//

#ifndef ARMCAN_SD_DRIVER_H
#define ARMCAN_SD_DRIVER_H

#include "fatfs.h"
namespace SD_Driver_IRQ {
    extern "C" void EXTI15_10_IRQHandler(void);
}
class CSdDriver {
    friend void SD_Driver_IRQ::EXTI15_10_IRQHandler(void);
public:
    CSdDriver();
    bool initSdDriver();
    bool deInitSdDriver();
    bool isSdConnected();
    uint32_t getSDFreeSpace();
    uint32_t getSDTotalSpace();
    uint32_t getSDState(char *text, uint32_t* sdStateCode);
    uint32_t getSDCardState(char *text, uint32_t* sdCardStateCode);
    bool mountSDCard();
    bool unmountSDCard();
    bool formatFAT32();
    bool saveScreenshot();
private:
    HAL_SD_CardStateTypeDef sdCardState;
    HAL_SD_StateTypeDef sdState;
    bool sdDetected = false;
    uint8_t buffer[_MAX_SS];
    int imageCounter = 0;

};

extern CSdDriver sdDriver;
#endif //ARMCAN_SD_DRIVER_H
