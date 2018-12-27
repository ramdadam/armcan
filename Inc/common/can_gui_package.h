#ifndef CAN_GUI_PACKAGE_H_
#define CAN_GUI_PACKAGE_H_

typedef struct
{
    gfxQueueGSyncItem q_item;
    uint16_t id;
    uint8_t dlc;
    uint8_t isRemote;
    union {
        uint8_t data_b[8];  //!< data as seen in bytes
        uint32_t data_w[2]; //!< data as two 32-bit words
        uint64_t data_l;    //!< data as 64-bit number
    } data;
    uint32_t count = 0;
    uint32_t cycle = 0;
    void* timer = nullptr;
    char displayText[100];
    uint64_t lastTick = 0;
    uint16_t dataPosStart = 0;
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

void bumpPackageCounter(can_gui_package* package);
void packageToString(can_gui_package* package);

void buildStringInCanGuiPackage(can_gui_package *package);

can_gui_package* convertCANFormDataToGuiPackage(can_gui_form_data* package);

#endif
