#ifndef __MAIN_VIEW_H
#define __MAIN_VIEW_H

#include "gfx.h"
#include "can_gui_package.h"
#include "event_listener.h"
#include "edit_can_message.h"
#include "notification_helper.h"
#include "gwin_table.h"
#include <cstdio>
#include "WidgetStyles.h"

#include "can_status_view.h"
#include "can_settings_view.h"
#include "can_view.h"
#include "tx_can_view.h"
#include "rx_can_view.h"
#include "add_can_message.h"
#include "main_view.h"
#include "logger.h"
#include "can_driver.h"

gfxQueueGSync *canTransmitQueue = nullptr;
gfxQueueGSync *canReceiveQueue = nullptr;
gfxQueueGSync *canReceiveQueue2 = nullptr;

void CMainView::createTable() {
    txView.createTxCanViewTable(&txTabPage);
    rxView.createRxCanViewTable(&rxTabPage);
    canSettingsView.createSettingsPage(&canSettingsTabPage);
    canStatusView.createStatusPage(&canStatusTabPage);
}

void CMainView::createTabset() {
    GWidgetInit wi;
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;

    wi.g.width = 480;
    wi.g.height = 272;
    wi.g.x = 0;
    wi.g.y = 0;
    ghTabset = gwinTabsetCreate(nullptr, &wi, GWIN_TABSET_BORDER);
    canStatusTabPage = gwinTabsetAddTab(ghTabset, "CAN Status", 1);
    txTabPage = gwinTabsetAddTab(ghTabset, "Transmit", 1);
    rxTabPage = gwinTabsetAddTab(ghTabset, "Receive", 1);
    canSettingsTabPage = gwinTabsetAddTab(ghTabset, "CAN Settings", 1);
    createTable();
}

void CMainView::showMainpage() {
    gwinShow(ghTabset);
}

void CMainView::hideMainpage() {
    gwinHide(ghTabset);
}

void CMainView::addRxCanPackage(can_gui_package* package) {
    rxView.putRxCanPackage(package);
}

void CMainView::triggerRxRedraw() {
    rxView.redrawTable();
}

void CMainView::triggerTxRedraw() {
    txView.redrawTable();
}

void CMainView::triggerCanSettingsUpdate() {
    canSettingsView.refreshSettings();
}

void CMainView::triggerCanStatusUpdate() {
    canStatusView.refreshView();
}

void CMainView::refreshActiveTab() {
    if(disableActiveTabRefresh) {
        return;
    }
    if(gwinGetVisible(txTabPage)) {
        triggerTxRedraw();
    } else if(gwinGetVisible(rxTabPage)){
        triggerRxRedraw();
    } else if(gwinGetVisible(canSettingsTabPage)){
        triggerCanSettingsUpdate();
    }else if(gwinGetVisible(canStatusTabPage)){
        triggerCanStatusUpdate();
    }
}

void redrawTables(void* cMainViewParam) {
    CMainView* cMainView = (CMainView*) cMainViewParam;
    cMainView->refreshActiveTab();
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

    canDriver.MX_CAN1_Init(50, false);
    while (true) {
        pe = geventEventWait(&gl, TIME_INFINITE);

        volatile EVENT_ACTION action = NO_ACTION;
        action = addMessageView.evalEvent(pe, action);
        action = txView.evalEvent(pe, action);
        action = canSettingsView.evalEvent(pe, action);
        action = rxView.evalEvent(pe, action);
        action = editMessageView.evalEvent(pe, action);

        addMessageView.performAction(action, pe);
        canSettingsView.performAction(action, pe);
        txView.performAction(action, pe);
        rxView.performAction(action, pe);
        editMessageView.performAction(action, pe);
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
                if (addMessageView.getFormData(&formData) == 0) {
                    break;
                }
                can_gui_package *package = convertCANFormDataToGuiPackage(&formData);
                buildStringInCanGuiPackage(package);
                if (txView.putTxCanPackage(package, TRUE) != -1) {
                    addMessageView.hideAddFrame();
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
                can_gui_package *tempCanPackage = txView.getTxSelectedCANPackage();
                if (tempCanPackage != nullptr) {
                    editMessageView.editCanMessage(tempCanPackage, FALSE);
                    hideMainpage();
                }
                break;
            }
        }
    }
}


#endif
