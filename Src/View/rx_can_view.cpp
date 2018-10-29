#include "gfx.h"
#include "can_gui_package.h"
#include "can_view.h"
#include "rx_can_view.h"
#include <stdio.h>
#include "notification_helper.h"

GHandle rxTable;
extern gfxQueueGSync* canReceiveQueue;

threadreturn_t CANReceiveThread(void* param)
{
    CRxCanView* cRxCanView = (CRxCanView*) param;
 	while (1) {
        const gfxQueueGSyncItem* package_queued = gfxQueueGSyncPop(canReceiveQueue, TIME_INFINITE);
        if(package_queued != 0){
            can_gui_package *canGuiPackage = (can_gui_package*)package_queued;
            cRxCanView->putRxCanPackage(canGuiPackage);
            delete canGuiPackage;
        }
	}
}

void CRxCanView::createRxCanViewTable(GHandle *parent)
{
    rxTable = createBaseTableWidget(parent);
    rxCanContainer = (can_gui_package_array)gfxAlloc(RX_MAX_PACKAGES * sizeof(can_gui_package *));
    gfxThreadCreate(NULL, 128, NORMAL_PRIORITY, CANReceiveThread, this);
}

void CRxCanView::deleteRxCanViewTable()
{
    deleteTableWidget();
}

void CRxCanView::syncRxList()
{
    gwinListDeleteAll(rxTable);
    for (uint16_t i = 0; i < rxCanContainerSize; i++)
    {
        gwinListAddItem(rxTable, rxCanContainer[i]->displayText, 0);
    }
}

int8_t CRxCanView::putRxCanPackage(can_gui_package *package)
{
    bool found = FALSE;
    for (uint16_t i = 0; i < rxCanContainerSize; i++)
    {
        can_gui_package *temp = rxCanContainer[i];
        if (temp && temp->id == package->id && temp->isRemote == package->isRemote)
        {
            found = TRUE;
            bumpPackageCounter(temp);
            //gwinRedraw(rxTable);
            return 0;
        }
    }
    if (!found)
    {
        if (rxCanContainerSize < RX_MAX_PACKAGES)
        {
            rxCanContainer[rxCanContainerSize] = package;
            package->count = 1;
            rxCanContainerSize += 1;
        }
    }
    syncRxList();
}
