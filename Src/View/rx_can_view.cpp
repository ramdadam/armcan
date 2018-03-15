#include "gfx.h"
#include "can_view.h"
#include "can_gui_package.h"

void createRxCanViewTable(GHandle *parent)
{
    createBaseTableWidget(parent);
    rxCanContainer = gfxAlloc(RX_MAX_PACKAGES * sizeof(can_gui_package*));
}

void deleteRxCanViewTable()
{
    deleteTableWidget();
}