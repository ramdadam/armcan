#ifndef __MAIN_VIEW_H
#define __MAIN_VIEW_H

#include "gfx.h"
#include "can_gui_package.h"
#include "event_listener.h"
#include "edit_can_message.h"
#include "notification_helper.h"
#include "gwin_table.h"
#include <stdio.h>
#include "can_settings_view.h"
#include "can_view.h"
#include "tx_can_view.h"
#include "rx_can_view.h"
#include "add_can_message.h"

#include "main_view.h"
#include "logger.h"


gfxQueueGSync *canTransmitQueue = nullptr;
gfxQueueGSync *canReceiveQueue = nullptr;

void CMainView::createTable() {
    static bool firstRun = true;
    if (!firstRun) {
        cTxCanView.deleteTxCanViewTable();
        cRxCanView.deleteRxCanViewTable();
    }
    cTxCanView.createTxCanViewTable(&tabset_page_1);
    cRxCanView.createRxCanViewTable(&tabset_page_2);
    cCanSettingsPage.createSettingsPage(&canSettingsPage);
    firstRun = false;
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
    tabset_page_2 = gwinTabsetAddTab(ghTabset, "Receive", 1);
    tabset_page_1 = gwinTabsetAddTab(ghTabset, "Transmit", 1);
    canSettingsPage = gwinTabsetAddTab(ghTabset, "Settings", 1);
    createTable();
}

void CMainView::showMainpage() {
    gwinShow(ghTabset);
    gwinShow(cTxCanView.getAddButton());
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

void CMainView::triggerSettingsUpdate() {
    cCanSettingsPage.updateHeapLabel();
}

void redrawTables(void* cMainViewParam) {
    CMainView* cMainView = (CMainView*) cMainViewParam;
    cMainView->triggerRxRedraw();
    cMainView->triggerTxRedraw();
    cMainView->triggerSettingsUpdate();
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

    while (true) {
        pe = geventEventWait(&gl, TIME_INFINITE);

        EVENT_ACTION action = NO_ACTION;
        LOG("\n\naction handler start");
        action = cAddCanMessageView.evalEvent(pe, action);
        LOG_NUMBER("cAddCanMessageView action Handler: %d", action);
        action = cTxCanView.evalEvent(pe, action);
        LOG_NUMBER("cTxCanView action Handler: %d", action);
        action = cEditMessageView.evalEvent(pe, action);
        LOG_NUMBER("cEditMessageView action Handler: %d", action);
        action = cCanSettingsPage.evalEvent(pe, action);
        LOG_NUMBER("cCanSettingsPage action Handler: %d", action);

        cAddCanMessageView.performAction(action, pe);
        cTxCanView.performAction(action, pe);
        cEditMessageView.performAction(action, pe);
        cCanSettingsPage.performAction(action, pe);
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
        }
    }
}


#endif
