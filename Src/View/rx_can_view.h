#ifndef RX_CAN_VIEW_H_
#define RX_CAN_VIEW_H_

#include "can_gui_package.h"

static can_gui_package_array rxCanContainer = 0;
static uint32_t rxCanContainerSize = 0;
void createRxCanViewTable(GHandle* parent);
void deleteRxCanViewTable();
void putRxCanPackage(can_gui_package* package, uint8_t allowPackageDeletion = TRUE );

#endif

// void deleteCANMessage(uint32_t id);
// void addCanMessageT oRXView(uint32_t id, can_gui_package *package);
