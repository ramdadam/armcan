#include "gfx.h"
#include "notification_helper.h"
#include "event_listener.h"

#include "can_gui_package.h"
#include "can_view.h"
#include "Inc/View/tx_can_view.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Inc/View/tx_can_view.h>
#include "can_driver.h"
#include "sd_driver.h"

#include "ImagePushButton.h"

EVENT_ACTION CTxCanView::evalEvent(GEvent *gEvent, EVENT_ACTION currentAction) {
    switch (gEvent->type) {
        case GEVENT_GWIN_BUTTON: {
            GWindowObject *target = ((GEventGWinButton *) gEvent)->gwin;

            if (target == ghTxEditButton) {
                return SHOW_EDIT_VIEW;
            } else if (target == ghDeleteTXItemButton) {
                return DELETE_TX_ITEM;
            } else if (target == ghAddButton) {
                return SHOW_ADD_VIEW;
            } else if (target == ghRepeatOneButton) {
                return SEND_ONE_MESSAGE;
            } else if (target == ghScreenshotButton) {
                return TAKE_TX_SCREENSHOT;
            }
            return currentAction != NO_ACTION ? currentAction : NO_ACTION;
        }
        case GEVENT_GWIN_LIST: {
            GWindowObject *target = ((GEventGWinList *) gEvent)->gwin;

            if(target ==table ) {
                int16_t index = gwinListGetSelected(table);
                if (index > txCanContainerSize) {
                    return NO_ACTION;
                } else if (index < 0) {
                    return NO_ACTION;
                } else {
                    return TX_ITEM_SELECTED;
                }
            }
        }
        default:
            return currentAction != NO_ACTION ? currentAction : NO_ACTION;
    }
}

EVENT_ACTION_STATUS CTxCanView::performAction(EVENT_ACTION action, GEvent *gEvent) {
    switch(action) {
        case SHOW_EDIT_VIEW:
        case SHOW_ADD_VIEW: {
            hideAllActionButtons();
            break;
        }
        case CLOSE_EDIT_VIEW:
        case CLOSE_ADD_VIEW:
        case ACCEPT_EDIT: {
            showAllActionButtons();
            int16_t index = gwinListGetSelected(table);
            checkRepeatBtnVisibility(index);
            break;
        }
        case ADD_MESSAGE:
        {
            showAllActionButtons();
            break;
        }
        case TX_ITEM_SELECTED: {
            gwinShow(ghTxEditButton);
            int16_t index = gwinListGetSelected(table);
            checkRepeatBtnVisibility(index);
            break;
        }
        case SEND_ONE_MESSAGE: {
            int16_t index = gwinListGetSelected(table);
            if(index >= 0 && txCanContainerSize > 0) {
                can_gui_package *temp = txCanContainer[index];
                canDriver.sendCANPackage(temp);
            }
            break;
        }
        case DELETE_TX_ITEM: {
            //TODO: extract event handling
            int16_t index = gwinListGetSelected(table);
            //TODO: add mutex to prevent data corruption
            if(index >= 0 && txCanContainerSize > 0) {

                can_gui_package* temp = txCanContainer[index];
                for(int16_t i = index; i<txCanContainerSize; i++) {
                    txCanContainer[i] = txCanContainer[i+1];
                }

                //TODO: extract with edit can message timer deletion logic
                if (temp->timer != nullptr) {
                    auto *timer = (GTimer *) temp->timer;

                    gtimerJab(timer);
                    gtimerDeinit(timer);
                    gfxFree(temp->timer);
                }
                gwinListItemDelete(table, index);
                gfxFree(temp);

                txCanContainerSize-=1;
                if(txCanContainerSize == 0) {

                    gwinHide(ghTxEditButton);
                    gwinHide(ghDeleteTXItemButton);
                }
                int16_t index = gwinListGetSelected(table);
                checkRepeatBtnVisibility(index);
            }
        }
    }
    return EVENT_HANDLED;
}

void CTxCanView::createButtonGroup(GHandle *parent) {
    GWidgetInit wi;
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 40;
    wi.g.height = 44;
    wi.g.parent = *parent;
    wi.g.x = 430;
    wi.g.y = 222;
    wi.text = "+";
    ghAddButton = gwinButtonCreate(NULL, &wi);

    gwinWidgetClearInit(&wi);
    wi.g.show = FALSE;
    wi.g.width = 40;
    wi.g.height = 44;
    wi.g.x = 350;
    wi.g.parent = *parent;
    wi.g.y = 222;
    wi.text = "-";
    ghDeleteTXItemButton = gwinButtonCreate(NULL, &wi);

    gwinWidgetClearInit(&wi);
    wi.g.show = false;
    wi.g.width = 32;
    wi.g.height = 32;
    wi.g.x = 250;
    wi.g.parent = *parent;
    wi.g.y = 220;
    iconRepeatOne = loadImageFromRomFs(REPEAT_ONE_IMAGE);
    repeatOneButtonParameter.iconHover = iconRepeatOne;
    repeatOneButtonParameter.iconEnabled = iconRepeatOne;
    ghRepeatOneButton = createImagePushButton(&wi, &repeatOneButtonParameter);

    gwinWidgetClearInit(&wi);
    wi.g.show = true;
    wi.g.width = 32;
    wi.g.height = 32;
    wi.g.x = 10;
    wi.g.parent = *parent;
    wi.g.y = 220;
    iconScreenshot = loadImageFromRomFs(SCREENSHOT_IMAGE);
    iconPressedScreenshot = loadImageFromRomFs(SCREENSHOT_PRESSED_IMAGE);
    screenshotButtonParameter.iconHover = iconPressedScreenshot;
    screenshotButtonParameter.iconEnabled = iconScreenshot;
    ghScreenshotButton = createImagePushButton(&wi, &screenshotButtonParameter);

    gwinWidgetClearInit(&wi);
    wi.g.show = false;
    wi.g.width = 32;
    wi.g.height = 32;
    wi.g.x = 210;
    wi.g.parent = *parent;
    wi.g.y = 220;
    wi.text = nullptr;
    iconEdit = loadImageFromRomFs(EDIT_IMAGE);
    editButtonParameter.iconHover = nullptr;
    editButtonParameter.iconEnabled = iconEdit;
    ghTxEditButton = createImagePushButton(&wi, &editButtonParameter);
}

GHandle CTxCanView::createTxCanViewTable(GHandle *parent) {
    table = createBaseTableWidget(parent, 480, 195);
    createButtonGroup(parent);
    txCanContainer = (can_gui_package_array) gfxAlloc(TX_MAX_PACKAGES * sizeof(can_gui_package *));

    GWidgetInit wi;
    gwinWidgetClearInit(&wi);
    wi.g.show = false;
    wi.g.width = 0;
    wi.g.height = 15;
    wi.g.parent = *parent;
    wi.g.x = 350;
    wi.g.y = 2;
    wi.text = "Please wait...";
    ghPleaseWaitLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghPleaseWaitLabel, gdispOpenFont("DejaVuSans16"));

    return table;
}

void CTxCanView::deleteTxCanViewTable() {
    deleteTableWidget();
    gwinDestroy(ghAddButton);
    gwinDestroy(ghDeleteTXItemButton);
}

void CTxCanView::syncList() {
    gwinListDeleteAll(table);
    for (uint16_t i = 0; i < txCanContainerSize; i++) {
        gwinListAddItem(table, txCanContainer[i]->displayText, 0);
    }
}

int8_t CTxCanView::putTxCanPackage(can_gui_package *package, uint8_t allowPackageDeletion = TRUE) {
    uint8_t found = FALSE;
    for (uint16_t i = 0; i < txCanContainerSize; i++) {
        can_gui_package *temp = txCanContainer[i];
        if (temp && temp->id == package->id && temp->isRemote == package->isRemote) {
            // temp->count += 1;
            if (allowPackageDeletion) {
                gfxFree(package);
            }
            found = TRUE;
            showMessage(" ID existiert bereits ");
            return -1;
        }
    }
    if (!found) {
        if (txCanContainerSize < TX_MAX_PACKAGES) {
            txCanContainer[txCanContainerSize] = package;
            txCanContainerSize += 1;
            syncList();
        }
    } else {
        syncList();
    }
}

can_gui_package *CTxCanView::getTxSelectedCANPackage() {
    int16_t index = gwinListGetSelected(table);
    if (index > txCanContainerSize) {
        return nullptr;
    } else if (index == -1) {
        return nullptr;
    } else {
        return txCanContainer[index];
    }
}

void CTxCanView::checkRepeatBtnVisibility(int rowIndex) {
    //TODO: add mutex to prevent data corruption
    if(rowIndex >= 0 && txCanContainerSize > 0) {
        can_gui_package *temp = txCanContainer[rowIndex];
        if(temp->cycle > 0 || temp->timer != nullptr) {
            gwinHide(ghRepeatOneButton);
        } else {
            gwinShow(ghRepeatOneButton);
        }
    } else {
        gwinHide(ghRepeatOneButton);
    }
}

void CTxCanView::showAllActionButtons() {
    gwinShow(ghScreenshotButton);
    gwinShow(ghTxEditButton);
    gwinShow(ghAddButton);
    gwinShow(ghDeleteTXItemButton);
    gwinShow(ghRepeatOneButton);
}

void CTxCanView::hideAllActionButtons() {
    gwinHide(ghTxEditButton);
    gwinHide(ghAddButton);
    gwinHide(ghDeleteTXItemButton);
    gwinHide(ghScreenshotButton);
    gwinHide(ghRepeatOneButton);
}

void CTxCanView::takeScreenshot() {
    gwinShow(ghPleaseWaitLabel);
    sdDriver.saveScreenshot();
    gwinHide(ghPleaseWaitLabel);
}
