#ifndef AC_VIEW_CAN_VIEW
#define AC_VIEW_CAN_VIEW

void createBaseTableWidget(char** header, uint32_t** colWidths, uint32_t colCount, GHandle* parent, uint16_t width = 480, uint16_t height = 272);
void deleteTableWidget();
#endif