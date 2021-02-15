//
// Created by mohamad on 04.08.19.
//

#ifndef ARMCAN_CAN_STATUS_VIEW_H
#define ARMCAN_CAN_STATUS_VIEW_H


class CCanStatusView : public CEventListener {
public:
    CCanStatusView();
    void createStatusPage(GHandle* parent);
    void refreshView();

    //EVENT_ACTION evalEvent(GEvent *event, EVENT_ACTION currentAction) override;

    //EVENT_ACTION_STATUS performAction(EVENT_ACTION action, GEvent *event) override;

private:
    void drawStatus(uint32_t x, uint32_t y, uint32_t color, GHandle parent);

    void buildRxFIFO0Group();
    void buildRxFIFO1Group();
    void buildCanStatusGroup();

    void buildTX0Group();
    void buildTX1Group();
    void buildTX2Group();

    void buildPeriphalGroup();


    GHandle* parent;
    GHandle ghRXFifo0Container;
    GHandle ghRXFifo0ContainerLabel;
    GHandle ghRXFifo0OverrunLabel;

    GHandle ghRXFifo1Container;
    GHandle ghRXFifo1ContainerLabel;
    GHandle ghRXFifo1OverrunLabel;


    GHandle ghCanStatusContainer;
    GHandle ghCanStatusContainerLabel;
    GHandle ghAckErrorLabel;
    GHandle ghProtocolWarningLabel;
    GHandle ghErrorPassiveLabel;
    GHandle ghBusOffErrorLabel;
    GHandle ghStuffErrorLabel;
    GHandle ghFormErrorLabel;
    GHandle ghBitRecessiveErrorLabel;
    GHandle ghBitDominantLErrorLabel;
    GHandle ghParameterErrorLabel;
    GHandle ghTimeOutErrorLabel;

    GHandle ghPeripheralNotStartedErrorLabel;
    GHandle ghPeripheralNotReadyErrorLabel;
    GHandle ghPeripheralNotInitErrorLabel;

    GHandle ghTX0Container;
    GHandle ghTX0ContainerLabel;
    GHandle ghTX0ArbLostErrorLabel;
    GHandle ghTX0TransmitErrorLabel;

    GHandle ghTX1Container;
    GHandle ghTX1ContainerLabel;
    GHandle ghTX1ArbLostErrorLabel;
    GHandle ghTX1TransmitErrorLabel;

    GHandle ghResetButton;

    GHandle ghTX2Container;
    GHandle ghTX2ContainerLabel;
    GHandle ghTX2ArbLostErrorLabel;
    GHandle ghTX2TransmitErrorLabel;

    font_t font;
};

#endif //ARMCAN_CAN_STATUS_VIEW_H
