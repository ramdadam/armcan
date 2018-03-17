#ifndef RX_CAN_VIEW_H_
#define RX_CAN_VIEW_H_

#include "can_gui_package.h"

static can_gui_package_array rxCanContainer = 0;
void createRxCanViewTable(GHandle* parent);
void deleteRxCanViewTable();

#endif

// void deleteCANMessage(uint32_t id);
// void addCanMessageT oRXView(uint32_t id, can_gui_package *package);
