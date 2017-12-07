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

void createTxCanViewTable(GHandle* parent) {
    const uint8_t colCount = TX_CAN_TABLE_COL_COUNT;
    #ifndef __cplusplus
        char **header = (char *[]){"ID(hex)", "DLC", "Data"};
        uint32_t** columnWidths = (const uint32_t *[]){60, 100, 140};
    #else
        static char header_temp1[] = {"ID(hex)"};
        static char header_temp2[] = {"DLC"};
        static char header_temp3[] = {"Data"};
        char* header[3] = {header_temp1, header_temp2, header_temp3};
        static uint32_t col1 = 60;
        static uint32_t col2 = 100;
        static uint32_t col3 = 140;
        uint32_t* columnWidths[3] = {&col1, &col2, &col3};
    #endif
    GHandle* table = createBaseTableWidget(header, columnWidths, colCount, parent, 480, 220);
    createButtonGroup(parent);
}

void deleteTxCanViewTable() {
    deleteTableWidget();
    gwinDestroy(ghAddButton);
    gwinDestroy(ghRemoveButton);
}
