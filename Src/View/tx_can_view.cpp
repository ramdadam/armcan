#include "gfx.h"
#include "notification_helper.h"
#include "can_gui_package.h"
#include "can_view.h"
#include "tx_can_view.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ImagePushButton.h"
#define TX_CAN_TABLE_COL_COUNT 3

GHandle table;
GHandle ghAddButton;
GHandle ghDeleteTXItemButton;
GHandle ghTxEditButton;
gdispImage iconEdit;


void createButtonGroup(GHandle *parent)
{
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
    wi.g.show = TRUE;
    wi.g.width = 27;
    wi.g.height = 27;
    wi.g.x = 1;
    wi.g.parent = *parent;	
    wi.g.y = 225;
    wi.text = 0;	
    ghTxEditButton = createImagePushButton(&wi, &iconEdit, EDIT_IMAGE);
}

GHandle createTxCanViewTable(GHandle *parent)
{
    table = createBaseTableWidget(parent, 480, 200);
    createButtonGroup(parent);
    txCanContainer = (can_gui_package_array)gfxAlloc(TX_MAX_PACKAGES * sizeof(can_gui_package *));
    return table;
}

void deleteTxCanViewTable()
{
    deleteTableWidget();
    gwinDestroy(ghAddButton);
    gwinDestroy(ghDeleteTXItemButton);
}

void syncList()
{
    gwinListDeleteAll(table);
    for (uint16_t i = 0; i < txCanContainerSize; i++)
    {
        gwinListAddItem(table, txCanContainer[i]->displayText, 0);
    }
}

int8_t putTxCanPackage(can_gui_package *package, uint8_t allowPackageDeletion = TRUE)
{
    uint8_t found = FALSE;
    for (uint16_t i = 0; i < txCanContainerSize; i++)
    {
        can_gui_package *temp = txCanContainer[i];
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
        if (txCanContainerSize < TX_MAX_PACKAGES)
        {
            txCanContainer[txCanContainerSize] = package;
            package->count = 1;
            txCanContainerSize += 1;
            syncList();
        }
    }
    else
    {
        syncList();
    }
}

can_gui_package *getTxSelectedCANPackage()
{
    uint16_t index = gwinListGetSelected(table);
    if (index > txCanContainerSize)
    {
        return 0;
    }
    else if (index == -1)
    {
        return 0;
    }
    else
    {
        return txCanContainer[index];
    }
}
