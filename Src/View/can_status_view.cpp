//
// Created by mohamad on 04.08.19.
//

#include "gfx.h"
#include "event_listener.h"
#include "can_status_view.h"
#include "can_gui_package.h"
#include "can_driver.h"
#include "sd_driver.h"

CCanStatusView::CCanStatusView() {

}
#define GROUP_MARGIN_Y 5
#define GROUP_LABEL_MARGIN_Y 20

void CCanStatusView::buildRxFIFO0Group() {
    GWidgetInit wi;

    gwinWidgetClearInit(&wi);

    wi.g.show = FALSE;
    wi.g.width = 125;
    wi.g.height = 25;
    wi.g.x = 10;
    wi.g.y = 30;
    wi.text = "RX FIFO 0";
    wi.g.parent = *parent;
    ghRXFifo0Container = gwinContainerCreate(nullptr, &wi, GWIN_CONTAINER_BORDER);
    wi.g.show = TRUE;

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 10;
    wi.g.y = 10;
    wi.g.parent = *parent;
    wi.text = "RX Mailbox 0";
    ghRXFifo0ContainerLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghRXFifo0ContainerLabel, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 3;
    wi.g.parent = ghRXFifo0Container;
    wi.text = "Bus Load: ";
    ghRXFifo0OverrunLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghRXFifo0OverrunLabel, font);
}

void CCanStatusView::buildRxFIFO1Group() {
    GWidgetInit wi;

    gwinWidgetClearInit(&wi);

    wi.g.show = FALSE;
    wi.g.width = 125;
    wi.g.height = 25;
    wi.g.x = 10;
    wi.g.y = 80;
    wi.text = "RX Mailbox 1";
    wi.g.parent = *parent;
    ghRXFifo1Container = gwinContainerCreate(nullptr, &wi, GWIN_CONTAINER_BORDER);
    wi.g.show = TRUE;

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 10;
    wi.g.y = 60;
    wi.g.parent = *parent;
    wi.text = "RX Mailbox 1";
    ghRXFifo1ContainerLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghRXFifo1ContainerLabel, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 3;
    wi.g.parent = ghRXFifo1Container;
    wi.text = "Bus Load: ";
    ghRXFifo1OverrunLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghRXFifo1OverrunLabel, font);
}

void CCanStatusView::buildCanStatusGroup() {

    GWidgetInit wi;

    gwinWidgetClearInit(&wi);
    uint16_t start = 0;
    uint16_t startGroup = start + 10 + GROUP_LABEL_MARGIN_Y;
    uint16_t startLabel = start + 10;
    wi.g.show = FALSE;
    wi.g.width = 200;
    wi.g.height = 90;
    wi.g.x = 150;
    wi.g.y = startGroup;
    wi.text = "CAN Status";
    wi.g.parent = *parent;
    ghCanStatusContainer = gwinContainerCreate(nullptr, &wi, GWIN_CONTAINER_BORDER);
    wi.g.show = TRUE;

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 150;
    wi.g.y = startLabel;
    wi.g.parent = *parent;
    wi.text = "CAN Status";
    ghCanStatusContainerLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghCanStatusContainerLabel, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 10;
    wi.g.y = 3;
    wi.g.parent = ghCanStatusContainer;
    wi.text = "Acknowledgment:";
    ghAckErrorLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghAckErrorLabel, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 10;
    wi.g.y = 23;
    wi.g.parent = ghCanStatusContainer;
    wi.text = "Stuff Bits:";
    ghStuffErrorLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghStuffErrorLabel, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 10;
    wi.g.y = 43;
    wi.g.parent = ghCanStatusContainer;
    wi.text = "Bus Status:";
    ghBusOffErrorLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghBusOffErrorLabel, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 10;
    wi.g.y = 63;
    wi.g.parent = ghCanStatusContainer;
    wi.text = "Frame Form:";
    ghFormErrorLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghFormErrorLabel, font);
}


void CCanStatusView::buildTX0Group() {
    GWidgetInit wi;
    gwinWidgetClearInit(&wi);

    wi.g.show = FALSE;
    wi.g.width = 150;
    wi.g.height = 50;
    wi.g.x = 10;
    wi.g.y = 130+GROUP_MARGIN_Y+GROUP_LABEL_MARGIN_Y;
    wi.text = "TX Mailbox 0 ";
    wi.g.parent = *parent;
    ghTX0Container = gwinContainerCreate(nullptr, &wi, GWIN_CONTAINER_BORDER);
    wi.g.show = TRUE;

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 10;
    wi.g.y = 130+GROUP_MARGIN_Y;
    wi.g.parent = *parent;
    wi.text = "TX Mailbox 0";
    ghTX0ContainerLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghTX0ContainerLabel, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 3;
    wi.g.parent = ghTX0Container;
    wi.text = "Arbitration Error:";
    ghTX0ArbLostErrorLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghTX0ArbLostErrorLabel, font);
    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 23;
    wi.g.parent = ghTX0Container;
    wi.text = "Transmit Error:";
    ghTX0TransmitErrorLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghTX0TransmitErrorLabel, font);
}
void CCanStatusView::buildTX1Group() {
    GWidgetInit wi;
    gwinWidgetClearInit(&wi);

    wi.g.show = FALSE;
    wi.g.width = 150;
    wi.g.height = 50;
    wi.g.x = 165;
    wi.g.y = 130+GROUP_MARGIN_Y+GROUP_LABEL_MARGIN_Y;
    wi.text = "TX Mailbox 1";
    wi.g.parent = *parent;
    ghTX1Container = gwinContainerCreate(nullptr, &wi, GWIN_CONTAINER_BORDER);
    wi.g.show = TRUE;

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 165;
    wi.g.y = 130+GROUP_MARGIN_Y;
    wi.g.parent = *parent;
    wi.text = "TX Mailbox 1";
    ghTX1ContainerLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghTX1ContainerLabel, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 3;
    wi.g.parent = ghTX1Container;
    wi.text = "Arbitration Error:";
    ghTX1ArbLostErrorLabel = gwinLabelCreate(nullptr, &wi);

    gwinSetFont(ghTX1ArbLostErrorLabel, font);
    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 23;
    wi.g.parent = ghTX1Container;
    wi.text = "Transmit Error:";
    ghTX1TransmitErrorLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghTX1TransmitErrorLabel, font);
}
void CCanStatusView::buildTX2Group() {
    GWidgetInit wi;
    gwinWidgetClearInit(&wi);

    wi.g.show = FALSE;
    wi.g.width = 150;
    wi.g.height = 50;
    wi.g.x = 320;
    wi.g.y = 130+GROUP_MARGIN_Y+GROUP_LABEL_MARGIN_Y;
    wi.text = "TX Mailbox 2";
    wi.g.parent = *parent;
    ghTX2Container = gwinContainerCreate(nullptr, &wi, GWIN_CONTAINER_BORDER);
    wi.g.show = TRUE;

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 320;
    wi.g.y = 130+GROUP_MARGIN_Y;
    wi.g.parent = *parent;
    wi.text = "TX Mailbox 2";
    ghTX2ContainerLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghTX2ContainerLabel, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 3;
    wi.g.parent = ghTX2Container;
    wi.text = "Arbitration Error:";
    ghTX2ArbLostErrorLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghTX2ArbLostErrorLabel, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 23;
    wi.g.parent = ghTX2Container;
    wi.text = "Transmit Error:";
    ghTX2TransmitErrorLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghTX2TransmitErrorLabel, font);


}


void CCanStatusView::createStatusPage(GHandle *parent) {
    this->parent = parent;
    font = gdispOpenFont("DejaVuSans12");

    buildRxFIFO0Group();
    buildRxFIFO1Group();
    buildCanStatusGroup();
    buildTX0Group();
    buildTX1Group();
    buildTX2Group();


    font_t font = gdispOpenFont("DejaVuSans20");
    GWidgetInit wi;
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 90;
    wi.g.height = 25;
    wi.g.parent = *parent;
    wi.g.x = 350;
    wi.g.y = 215;
    wi.text = "Reset Errors";
    ghResetButton = gwinButtonCreate(nullptr, &wi);


    gwinShow(ghRXFifo0Container);
    gwinShow(ghRXFifo1Container);
    gwinShow(ghCanStatusContainer);
    gwinShow(ghTX0Container);
    gwinShow(ghTX1Container);
    gwinShow(ghTX2Container);
    gwinRedraw(*parent);
    refreshView();
    sdDriver.saveScreenshot();
}

uint32_t getStatusColorForErrorFlag(uint32_t errorFlag) {
    return canDriver.hasError(errorFlag) ? Red : SuccessGreen;
}

void CCanStatusView::drawStatus(uint32_t x, uint32_t y, uint32_t color, GHandle parent) {
    gwinSetColor(parent, Black);
    gwinDrawCircle(parent, x, y, 8);
    gwinSetColor(parent, color);
    gwinFillCircle(parent, x, y, 7);
    gwinSetColor(parent, Black);

}

void CCanStatusView::refreshView() {
    drawStatus(100,13, getStatusColorForErrorFlag(HAL_CAN_ERROR_RX_FOV0), ghRXFifo0Container);
    drawStatus(100,13, getStatusColorForErrorFlag(HAL_CAN_ERROR_RX_FOV1), ghRXFifo1Container);

    drawStatus(150, 15, getStatusColorForErrorFlag(HAL_CAN_ERROR_ACK), ghCanStatusContainer);
    drawStatus(150, 35, getStatusColorForErrorFlag(HAL_CAN_ERROR_BOF), ghCanStatusContainer);
    drawStatus(150, 55, getStatusColorForErrorFlag(HAL_CAN_ERROR_STF), ghCanStatusContainer);
    drawStatus(150, 75, getStatusColorForErrorFlag(HAL_CAN_ERROR_FOR), ghCanStatusContainer);
//    drawStatus(150, 95, getStatusColorForErrorFlag(HAL_CAN_ERROR_CRC), ghCanStatusContainer);

    drawStatus(130, 15, getStatusColorForErrorFlag(HAL_CAN_ERROR_TX_ALST0), ghTX0Container);
    drawStatus(130, 35, getStatusColorForErrorFlag(HAL_CAN_ERROR_TX_TERR0), ghTX0Container);

    drawStatus(130, 15, getStatusColorForErrorFlag(HAL_CAN_ERROR_TX_ALST1), ghTX1Container);
    drawStatus(130, 35, getStatusColorForErrorFlag(HAL_CAN_ERROR_TX_TERR1), ghTX1Container);

    drawStatus(130, 15, getStatusColorForErrorFlag(HAL_CAN_ERROR_TX_ALST2), ghTX2Container);
    drawStatus(130, 35, getStatusColorForErrorFlag(HAL_CAN_ERROR_TX_TERR2), ghTX2Container);
}
//
//EVENT_ACTION CCanStatusView::evalEvent(GEvent *event, EVENT_ACTION currentAction) {
//    switch (event->type) {
//        case GEVENT_GWIN_BUTTON: {
//            GWindowObject *target = ((GEventGWinButton *) event)->gwin;
//
//            if(target == ghResetButton) {
//                return RESET_CAN_ERRORS;
//            }
//            break;
//        }
//        default: {
//            return currentAction != NO_ACTION ? currentAction : NO_ACTION;
//        }
//    }
//    return currentAction != NO_ACTION ? currentAction : NO_ACTION;
//}
//
//EVENT_ACTION_STATUS CCanStatusView::performAction(EVENT_ACTION action, GEvent *event) {
//
//    if(action == RESET_CAN_ERRORS) {
//        canDriver.resetCanErrors();
//        return EVENT_HANDLED;
//    }
//
//    return EVENT_NOT_HANDLED;
//}
