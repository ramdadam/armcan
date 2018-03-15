#include "gfx.h"
#include "can_view.h"
#include "tx_can_view.h"

#define TX_CAN_TABLE_COL_COUNT 3

void createButtonGroup(GHandle* parent) {
	GWidgetInit	wi;    
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
    wi.g.x = 400;
    wi.g.parent = *parent;
    wi.g.y = 225;
    wi.text = "-";
	ghRemoveButton = gwinButtonCreate(NULL, &wi);
}

GHandle* createTxCanViewTable(GHandle* parent) {
    GHandle* table = createBaseTableWidget(parent, 480, 200);
    createButtonGroup(parent);
    return table;
}

void deleteTxCanViewTable() {
    deleteTableWidget();
    gwinDestroy(ghAddButton);
    gwinDestroy(ghRemoveButton);
}
