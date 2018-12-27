#ifndef RX_CAN_VIEW_H_
#define RX_CAN_VIEW_H_
#define RX_MAX_PACKAGES 25

class CRxCanView : public CCanView {
public:
    can_gui_package_array rxCanContainer = 0;
    uint32_t rxCanContainerSize = 0;
    void createRxCanViewTable(GHandle* parent);
    void deleteRxCanViewTable();
    void syncRxList();
    int8_t putRxCanPackage(can_gui_package *package);
    friend threadreturn_t CANReceiveThread(void* param);
};

#endif

// void deleteCANMessage(uint32_t id);
// void addCanMessageT oRXView(uint32_t id, can_gui_package *package);
