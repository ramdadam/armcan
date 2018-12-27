#include "gfx.h"
#include "can_gui_package.h"
#include "can_view.h"
#include "Inc/View/rx_can_view.h"
#include <stdio.h>
#include <Inc/common/can_gui_package.h>
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
    createBaseTableWidget(parent);
    rxCanContainer = (can_gui_package_array) gfxAlloc(RX_MAX_PACKAGES * sizeof(can_gui_package *));
//    gfxThreadCreate(NULL, 128, NORMAL_PRIORITY, CANReceiveThread, this);
}

void CRxCanView::deleteRxCanViewTable() {
    deleteTableWidget();
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
            //gwinRedraw(rxTable);
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
//    syncRxList();
}
