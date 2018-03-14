#ifndef CAN_GUI_PACKAGE_H_
#define CAN_GUI_PACKAGE_H_

typedef struct
{
    uint16_t id;
    uint8_t dlc;
    uint8_t isRemote;
    union {
        uint8_t data_b[8];  //!< data as seen in bytes
        uint32_t data_w[2]; //!< data as two 32-bit words
        uint64_t data_l;    //!< data as 64-bit number
    } data;
    uint64_t count = 0;
    uint32_t cycle = 0;
    void* timer = 0;
    char displayText[100];
} can_gui_package;

typedef can_gui_package** can_gui_package_array;

typedef struct
{
    uint16_t id;
    uint8_t dlc;
    uint8_t isRemote;
    union {
        uint8_t data_b[8];  //!< data as seen in bytes
        uint32_t data_w[2]; //!< data as two 32-bit words
        uint64_t data_l;    //!< data as 64-bit number
    } data;
} can_gui_form_data;


void buildStringInCanGuiPackage(can_gui_package *package);

can_gui_package* convertCANFormDataToGuiPackage(can_gui_form_data* package);

#endif