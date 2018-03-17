#include "gfx.h"
#include "can_view.h"
#include "rx_can_view.h"

#define RX_MAX_PACKAGES 25

void createRxCanViewTable(GHandle *parent)
{
    createBaseTableWidget(parent);
    rxCanContainer = (can_gui_package_array) gfxAlloc(RX_MAX_PACKAGES * sizeof(can_gui_package*));
}

void deleteRxCanViewTable()
{
    deleteTableWidget();
}