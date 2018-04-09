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

void putRxCanPackage(can_gui_package* package, uint8_t allowPackageDeletion ) {
    uint8_t found = FALSE;
    for(uint16_t i = 0; i<rxCanContainerSize; i++) {
        can_gui_package* temp = rxCanContainer[rxCanContainerSize];
        if(temp && temp->id == package->id && temp->isRemote == package->isRemote) {
            temp->count += 1;
            if(allowPackageDeletion) {
                gfxFree(package);
            }
            found = TRUE;
            break;
        }
    }
    if(!found){
        if(rxCanContainerSize < RX_MAX_PACKAGES) {
            rxCanContainer[rxCanContainerSize] = package;
            package->count = 1;
        }
    }
}