
#include "can_gui_package.h"

void createRxCanViewTable(GHandle* parent);
void deleteRxCanViewTable();


void deleteCANMessage(uint32_t id);
void addCanMessageToRXView(uint32_t id, can_gui_package *package);
