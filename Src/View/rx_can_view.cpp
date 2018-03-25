#include "gfx.h"
#include "can_gui_package.h"
#include "can_view.h"
#include "rx_can_view.h"
#include "notification_helper.h"

GHandle rxTable;
void createRxCanViewTable(GHandle *parent)
{
    rxTable = createBaseTableWidget(parent);
    rxCanContainer = (can_gui_package_array)gfxAlloc(RX_MAX_PACKAGES * sizeof(can_gui_package *));
}

void deleteRxCanViewTable()
{
    deleteTableWidget();
}

void addRXCanMessage(can_gui_package *package) {

}

void syncRxList()
{
    gwinListDeleteAll(rxTable);
    for (uint16_t i = 0; i < rxCanContainerSize; i++)
    {
        gwinListAddItem(rxTable, rxCanContainer[i]->displayText, 0);
    }
}

int8_t putRxCanPackage(can_gui_package *package, uint8_t allowPackageDeletion = TRUE)
{
    uint8_t found = FALSE;
    for (uint16_t i = 0; i < rxCanContainerSize; i++)
    {
        can_gui_package *temp = rxCanContainer[i];
        if (temp && temp->id == package->id && temp->isRemote == package->isRemote)
        {
            // temp->count += 1;
            if (allowPackageDeletion)
            {
                gfxFree(package);
            }
            found = TRUE;
            showMessage(" ID existiert bereits ");
            return -1;
        }
    }
    if (!found)
    {
        if (rxCanContainerSize < RX_MAX_PACKAGES)
        {
            rxCanContainer[rxCanContainerSize] = package;
            package->count = 1;
            rxCanContainerSize += 1;
            syncRxList();
        }
    }
    else
    {
        syncRxList();
    }
}