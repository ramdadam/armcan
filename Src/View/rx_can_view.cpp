#include "gfx.h"
#include "can_view.h"
#include "can_gui_package.h"
#define RX_CAN_TABLE_COL_COUNT 4
void createRxCanViewTable(GHandle *parent)
{
    const uint8_t colCount = RX_CAN_TABLE_COL_COUNT;
#ifndef __cplusplus
    char **header = (char *[]){"ID(hex)", "DLC", "Data"};
    uint32_t **columnWidths = (const uint32_t *[]){60, 100, 140};
#else
    static char header_temp1[] = {"ID(hex)"};
    static char header_temp2[] = {"DLC"};
    static char header_temp3[] = {"Data"};
    char *header[3] = {header_temp1, header_temp2, header_temp3};
    static uint32_t col1 = 60;
    static uint32_t col2 = 100;
    static uint32_t col3 = 140;
    uint32_t *columnWidths[3] = {&col1, &col2, &col3};
#endif
    createBaseTableWidget(header, columnWidths, colCount, parent);
}


void deleteRxCanViewTable()
{
    deleteTableWidget();
}