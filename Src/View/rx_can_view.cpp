#include "gfx.h"
#include "can_view.h"
#include "rx_can_view.h"

void createRxCanViewTable(GHandle *parent)
{
    createBaseTableWidget(parent);
}

void deleteRxCanViewTable()
{
    deleteTableWidget();
}
