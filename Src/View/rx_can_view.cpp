#include "gfx.h"
#include "can_gui_package.h"
#include "ImagePushButton.h"
#include "can_view.h"
#include <Inc/events/event_listener.h>
#include "Inc/View/rx_can_view.h"
#include <stdio.h>
#include <Inc/View/rx_can_view.h>
#include "sd_driver.h"

#include "notification_helper.h"

extern gfxQueueGSync *canReceiveQueue;

threadreturn_t CANReceiveThread(void *param) {
    CRxCanView *cRxCanView = (CRxCanView *) param;
    while (1) {
        const gfxQueueGSyncItem *package_queued = gfxQueueGSyncPop(canReceiveQueue, TIME_INFINITE);
        if (package_queued != 0) {
            can_gui_package *canGuiPackage = (can_gui_package *) package_queued;
            cRxCanView->putRxCanPackage(canGuiPackage);
        }
    }
}

void CRxCanView::createRxCanViewTable(GHandle *parent) {
    createBaseTableWidget(parent, 480, 195);
    rxCanContainer = (can_gui_package_array) gfxAlloc(RX_MAX_PACKAGES * sizeof(can_gui_package *));
    createButtonGroup(parent);
}

void CRxCanView::syncRxList() {
    gwinListDeleteAll(table_view);
    for (uint16_t i = 0; i < rxCanContainerSize; i++) {
        gwinListAddItem(table_view, rxCanContainer[i]->displayText, 0);
    }
}

int8_t CRxCanView::putRxCanPackage(can_gui_package *package) {
    bool found = FALSE;
    for (uint16_t i = 0; i < rxCanContainerSize; i++) {
        volatile can_gui_package *temp = rxCanContainer[i];
        if (temp && temp->id == package->id && temp->isRemote == package->isRemote &&
            temp->data.data_l == package->data.data_l) {
            found = TRUE;
            temp->count += 1;
            bumpPackageCounter((can_gui_package *) temp);
            delete package;
            return 0;
        }
    }
    if (!found) {
        if (rxCanContainerSize < RX_MAX_PACKAGES) {
            packageToString(package);
            rxCanContainer[rxCanContainerSize] = package;
            rxCanContainerSize += 1;
            syncRxList();
        }
    }
}

EVENT_ACTION CRxCanView::evalEvent(GEvent * gEvent, EVENT_ACTION currentAction) {
    switch (gEvent->type) {
        case GEVENT_GWIN_BUTTON: {
            GWindowObject *target = ((GEventGWinButton *) gEvent)->gwin;

            if (target == ghScreenshotButton) {
                return TAKE_RX_SCREENSHOT;
            } else if (target == ghClearBtn) {
                return CLEAR_RX_VIEW;
            }
            return currentAction != NO_ACTION ? currentAction : NO_ACTION;
        }
        default:
            return currentAction != NO_ACTION ? currentAction : NO_ACTION;
    }
}

EVENT_ACTION_STATUS CRxCanView::performAction(EVENT_ACTION action, GEvent * gEvent) {
    switch(action) {
        case CLEAR_RX_VIEW: {
            gwinListDeleteAll(table_view);
            for (uint16_t i = 0; i < rxCanContainerSize; i++) {
                delete rxCanContainer[i];
            }
            rxCanContainerSize = 0;
            break;
        }
    }
    return EVENT_HANDLED;
}

void CRxCanView::createButtonGroup(GHandle *parent) {
    font_t font = gdispOpenFont("DejaVuSans20");
    GWidgetInit wi;
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 70;
    wi.g.height = 44;
    wi.g.parent = *parent;
    wi.g.x = 400;
    wi.g.y = 222;
    ghClearBtn = gwinButtonCreate(nullptr, &wi);
    gwinSetFont(ghClearBtn, font);
    gwinSetText(ghClearBtn, "Clear", 1);

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
}

void CRxCanView::redrawTable() {
    CCanView::redrawTable();
    gwinRedraw(ghScreenshotButton);
    gwinRedraw(ghClearBtn);
}
