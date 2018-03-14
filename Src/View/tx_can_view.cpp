#include "gfx.h"
#include "can_gui_package.h"
#include "can_view.h"
#include "tx_can_view.h"
#include "notification_helper.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "romfs_files.h"
#define TX_CAN_TABLE_COL_COUNT 3

GHandle table;
GHandle ghAddButton;
GHandle ghRemoveButton;
GHandle ghTxEditButton;
gdispImage iconEdit;

void myButtonRendering(GWidgetObject* gw, void* param)
{
	const GColorSet* colors;
	// Get the icon/image
	gdispImage* icon = (gdispImage*)param;
	if (!icon) {
		return;
	}
	// Get the appropriate color pallete from the widget style
	if (!gwinGetEnabled((GHandle)gw))
		colors = &gw->pstyle->disabled;
	else if ((gw->g.flags & GBUTTON_FLG_PRESSED))
		colors = &gw->pstyle->pressed;
	else
		colors = &gw->pstyle->enabled;
	// Draw the basic rectangle with border
	gdispGFillArea(gw->g.display, gw->g.x+1, gw->g.y+1, gw->g.width-2, gw->g.height-2, colors->fill);
	gdispGDrawBox(gw->g.display, gw->g.x, gw->g.y, gw->g.width, gw->g.height, colors->edge);
	// Draw the string. Use StringBox() for proper justification and word-wrapping support
	gdispGDrawStringBox(gw->g.display, gw->g.x+gw->g.height, gw->g.y, gw->g.width-gw->g.height, gw->g.height, gw->text, gw->g.font, colors->text, justifyLeft);
	// Draw the image
	gdispGImageDraw(gw->g.display, icon, gw->g.x+(gw->g.height-icon->width)/2, gw->g.y+(gw->g.height-icon->height)/2, icon->width, icon->height, 0, 0);
}

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
    wi.g.show = TRUE;
    wi.g.width = 40;
    wi.g.height = 44;
    wi.g.x = 350;
    wi.g.parent = *parent;
    wi.g.y = 225;
    wi.text = "-";
    ghRemoveButton = gwinButtonCreate(NULL, &wi);
    	
    gdispImageOpenFile(&iconEdit, "out.bmp");
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 27;
    wi.g.height = 27;
    wi.g.x = 1;
    wi.g.parent = *parent;	
    wi.customDraw = myButtonRendering;	
    wi.customParam = &iconEdit;
    wi.g.y = 225;
    wi.text = 0;	
    ghTxEditButton = gwinButtonCreate(0, &wi);
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
    gwinDestroy(ghRemoveButton);
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
