#ifndef CAN_GUI_PACKAGE_H_
#define CAN_GUI_PACKAGE_H_
#include <stdint.h>
typedef struct
{
    uint16_t id;
    uint8_t dlc;
    uint8_t isRemote;
    union {
        uint8_t data_b[8];  //!< data as seen in bytes
        uint32_t data_w[2]; //!< data as two 32-bit words
        uint64_t data_l;    //!< data as 64-bit number
    };
    uint64_t count = 0;
    uint32_t cycle = 0;
} can_gui_package;

typedef struct
{
    uint16_t id;
    uint8_t dlc;
    uint8_t isRemote;
    union {
        uint8_t data_b[8];  //!< data as seen in bytes
        uint32_t data_w[2]; //!< data as two 32-bit words
        uint64_t data_l;    //!< data as 64-bit number
    };
} can_gui_form_data;

static void convertCanGuiPackageToString(can_gui_package* package, char* string) {
    
}

static can_gui_package* convertCANFormDataToGuiPackage(can_gui_form_data* package)
{
    can_gui_package *guiPackage = (can_gui_package*)gfxAlloc(sizeof(can_gui_package));
    guiPackage->id = package->id;
    guiPackage->dlc = package->dlc;
    guiPackage->data_l = package->data_l;
    return guiPackage;
}
#endif