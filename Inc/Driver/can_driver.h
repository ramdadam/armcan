//
// Created by mohamad on 20.12.18.
//

#ifndef ARMCAN_CAN_DRIVER_H
#define ARMCAN_CAN_DRIVER_H

#define DEFAULT_CAN_PRESCALER 50
#define DEFAULT_CAN_SPEED 5000000

namespace CAN_driver_ISR // need a namespace to declare friend functions
{
    extern "C" void CAN1_RX1_IRQHandler(void);
    extern "C" void CAN1_RX0_IRQHandler(void);
    extern "C" void CAN1_TX_IRQHandler(void);
}


class CCanDriver {
    friend void CAN_driver_ISR::CAN1_RX1_IRQHandler(void);
    friend void CAN_driver_ISR::CAN1_RX0_IRQHandler(void);
    friend void CAN_driver_ISR::CAN1_TX_IRQHandler(void);

public:
    CCanDriver();

    void MX_CAN1_Init(uint16_t prescaler, bool sleepMode);

    can_gui_package *receiveCANPackage();

    uint8_t sendCANPackage(can_gui_package *package);

    inline CAN_HandleTypeDef *getHandle() {
        return &hcan;
    }
    inline void clearMailboxPendingTx0(){
        msgPendingTx0 = false;
    }
    inline void clearMailboxPendingTx1(){
        msgPendingTx1 = false;
    }
    inline void clearMailboxPendingTx2(){
        msgPendingTx2 = false;
    }

    uint8_t hasError(uint32_t errorFlag);
    uint8_t getUserFriendlyErrorText(char* text, uint32_t* canErrorCode);
    uint8_t getUserFriendlyState(char* text, uint32_t* canState);

private:
    bool disableCAN = false;
    CAN_HandleTypeDef hcan;

    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];

    bool msgPendingTx0 = false;
    bool msgPendingTx1 = false;
    bool msgPendingTx2 = false;
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;

};

extern CCanDriver canDriver;

#endif //ARMCAN_CAN_DRIVER_H
