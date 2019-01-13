#ifndef RX_CAN_VIEW_H_
#define RX_CAN_VIEW_H_



#include "ImagePushButton.h"
#define RX_MAX_PACKAGES 25

class CRxCanView : public CCanView, public CEventListener  {
public:
    friend threadreturn_t CANReceiveThread(void* param);
    EVENT_ACTION evalEvent(GEvent*, EVENT_ACTION currentAction) override;
    EVENT_ACTION_STATUS performAction(EVENT_ACTION, GEvent*) override;

    int8_t putRxCanPackage(can_gui_package *package);

    void createButtonGroup(GHandle* parent);
    void createRxCanViewTable(GHandle* parent);

    void redrawTable() override;
private:
    GHandle ghClearBtn;
    GHandle ghScreenshotButton;
    void syncRxList();
    uint32_t rxCanContainerSize = 0;
    can_gui_package_array rxCanContainer = 0;

    gdispImage* iconScreenshot;
    gdispImage* iconPressedScreenshot;
    ImageButtonParameter screenshotButtonParameter;
};

#endif

// void deleteCANMessage(uint32_t id);
// void addCanMessageT oRXView(uint32_t id, can_gui_package *package);
