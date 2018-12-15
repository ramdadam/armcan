#ifndef __MAIN_VIEW_H
#define __MAIN_VIEW_H

#include "gfx.h"
#include "fatfs.h"

#include "can_gui_package.h"
#include "event_listener.h"
#include "edit_can_message.h"
#include "notification_helper.h"
#include "gwin_table.h"
#include <stdio.h>
#include "can_view.h"
#include "tx_can_view.h"
#include "rx_can_view.h"
#include "add_can_message.h"
#include "main_view.h"
#include "logger.h"


gfxQueueGSync *canTransmitQueue = 0;
gfxQueueGSync *canReceiveQueue = 0;

void CMainView::createTable(void) {
    static bool firstRun = true;
    if (!firstRun) {
        cTxCanView.deleteTxCanViewTable();
        cRxCanView.deleteRxCanViewTable();
    }
    cTxCanView.createTxCanViewTable(&tabset_page_1);
    cRxCanView.createRxCanViewTable(&tabset_page_2);
    firstRun = false;
}

void CMainView::createTabset(void) {
    GWidgetInit wi;
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;

    wi.g.width = 480;
    wi.g.height = 272;
    wi.g.x = 0;
    wi.g.y = 0;
    ghTabset = gwinTabsetCreate(0, &wi, GWIN_TABSET_BORDER);
    tabset_page_2 = gwinTabsetAddTab(ghTabset, "Receive", 1);
    tabset_page_1 = gwinTabsetAddTab(ghTabset, "Transmit", 1);
    createTable();
}

void CMainView::showMainpage() {
    gwinShow(ghTabset);
    gwinShow(cTxCanView.getAddButton());
    gwinShow(cTxCanView.getTXEditButton());
}

void CMainView::hideMainpage() {
    gwinHide(ghTabset);
    gwinHide(cTxCanView.getAddButton());
    gwinHide(cTxCanView.getTXEditButton());
}

void CMainView::addRxCanPackage(can_gui_package* package) {
    cRxCanView.putRxCanPackage(package);
}

void CMainView::triggerRxRedraw() {
    cRxCanView.redrawTable();

}
void CMainView::triggerTxRedraw() {
    cTxCanView.redrawTable();
}

void redrawTables(void* cMainViewParam) {
    CMainView* cMainView = (CMainView*) cMainViewParam;
    cMainView->triggerRxRedraw();
    cMainView->triggerTxRedraw();
}


void CMainView::initMainPage(void) {
    gfxInit();
    GEvent *pe;
    GListener gl;
    gwinSetDefaultFont(gdispOpenFont("DejaVuSans12"));
    gwinSetDefaultStyle(&WhiteWidgetStyle, FALSE);
    gdispClear(White);
    createTabset();
    initNotifications();
    geventListenerInit(&gl);
    gwinAttachListener(&gl);

    GTimer *redrawTimer = nullptr;
    gtimerInit(redrawTimer);
    gtimerStart(redrawTimer, redrawTables, this, TRUE, 1000);

    MX_FATFS_Init();
    while (1) {
        pe = geventEventWait(&gl, TIME_INFINITE);

        EVENT_ACTION action = NO_ACTION;
        LOG("\n\naction handler start");
        action = cAddCanMessageView.evalEvent(pe, action);
        LOG_NUMBER("cAddCanMessageView action Handler: %d", action);
        action = cTxCanView.evalEvent(pe, action);
        LOG_NUMBER("cTxCanView action Handler: %d", action);
        action = cEditMessageView.evalEvent(pe, action);
        LOG_NUMBER("cEditMessageView action Handler: %d", action);

        cAddCanMessageView.performAction(action, pe);
        cTxCanView.performAction(action, pe);
        cEditMessageView.performAction(action, pe);
        switch (action) {
            case CLOSE_ADD_VIEW: {
                showMainpage();
                break;
            }
            case SHOW_ADD_VIEW: {
                hideMainpage();
                break;
            }
            case ADD_MESSAGE: {
                can_gui_form_data formData;
                if (cAddCanMessageView.getFormData(&formData) == 0) {
                    break;
                }
                can_gui_package *package = convertCANFormDataToGuiPackage(&formData);
                buildStringInCanGuiPackage(package);
                if (cTxCanView.putTxCanPackage(package, TRUE) != -1) {
                    cAddCanMessageView.hideAddFrame();

                    showMainpage();
                }

                break;
            }
            case ACCEPT_EDIT: {
                showMainpage();
                break;
            }
            case CLOSE_EDIT_VIEW: {
                showMainpage();
                break;
            }
            case SHOW_EDIT_VIEW: {
                can_gui_package *tempCanPackage = cTxCanView.getTxSelectedCANPackage();
                if (tempCanPackage != 0) {
                    cEditMessageView.editCanMessage(tempCanPackage, FALSE);
                    hideMainpage();
                }
                break;
            }
            case DELETE_TX_ITEM: {
                can_gui_package *tempCanPackage = cTxCanView.getTxSelectedCANPackage();
                if (tempCanPackage->timer != 0) {
                    //TODO: dont delete timer in controller!
                    cEditMessageView.immediateDeleteTimer((GTimer *) tempCanPackage->timer);
                    tempCanPackage->timer = 0;
                }
                break;
            }
        }
    }
}


#endif
