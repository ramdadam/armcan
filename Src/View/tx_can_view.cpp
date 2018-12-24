#include "gfx.h"
#include "notification_helper.h"
#include "event_listener.h"

#include "can_gui_package.h"
#include "can_view.h"
#include "tx_can_view.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ImagePushButton.h"

#define TX_CAN_TABLE_COL_COUNT 3

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
        case ACCEPT_EDIT: {
            gwinShow(ghTxEditButton);
            gwinShow(ghDeleteTXItemButton);
            break;
        }
        case CLOSE_EDIT_VIEW: {
            gwinShow(ghTxEditButton);
            gwinShow(ghDeleteTXItemButton);
            break;
        }
        case ADD_MESSAGE:
        {
            gwinShow(ghTxEditButton);
            gwinShow(ghDeleteTXItemButton);
            break;
        }
        case TX_ITEM_SELECTED: {
            gwinShow(ghTxEditButton);
            gwinShow(ghDeleteTXItemButton);
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
    wi.g.x = 440;
    wi.g.y = 225;
    wi.text = "+";
    ghAddButton = gwinButtonCreate(NULL, &wi);

    gwinWidgetClearInit(&wi);
    wi.g.show = FALSE;
    wi.g.width = 40;
    wi.g.height = 44;
    wi.g.x = 350;
    wi.g.parent = *parent;
    wi.g.y = 225;
    wi.text = "-";
    ghDeleteTXItemButton = gwinButtonCreate(NULL, &wi);

    gwinWidgetClearInit(&wi);
    wi.g.show = false;
    wi.g.width = 27;
    wi.g.height = 27;
    wi.g.x = 225;
    wi.g.parent = *parent;
    wi.g.y = 225;
    wi.text = nullptr;
    ghTxEditButton = createImagePushButton(&wi, &iconEdit, EDIT_IMAGE);
}

GHandle CTxCanView::createTxCanViewTable(GHandle *parent) {
    table = createBaseTableWidget(parent, 480, 200);
    createButtonGroup(parent);
    txCanContainer = (can_gui_package_array) gfxAlloc(TX_MAX_PACKAGES * sizeof(can_gui_package *));
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
            package->count = 1;
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
