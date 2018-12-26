


#include <Inc/common/ImagePushButton.h>

#define TX_MAX_PACKAGES 25
class CTxCanView : public CCanView, public CEventListener {
public:
    EVENT_ACTION evalEvent(GEvent*, EVENT_ACTION currentAction) override;
    EVENT_ACTION_STATUS performAction(EVENT_ACTION, GEvent*) override;
    can_gui_package_array txCanContainer = 0;
    uint32_t txCanContainerSize = 0;
    void syncList();
    GHandle createTxCanViewTable(GHandle* parent);
    void deleteTxCanViewTable();
    can_gui_package* getTxSelectedCANPackage();
    void createButtonGroup(GHandle* parent);
    int8_t putTxCanPackage(can_gui_package* package, uint8_t allowPackageDeletion);

    /* GHandle getter */
    GHandle getAddButton() {
        return ghAddButton;
    }
    GHandle getDeleteTXItemButton() {
        return ghDeleteTXItemButton;
    }
    GHandle getTXEditButton() {
        return ghTxEditButton;
    }
private:
    GHandle table;
    GHandle ghAddButton;
    GHandle ghDeleteTXItemButton;
    GHandle ghTxEditButton;
    GHandle ghScreenshotButton;
    GHandle ghRepeatOneButton;


    gdispImage iconRepeatOne;
    ImageButtonParameter repeatOneButtonParameter;

    gdispImage iconEdit;
    ImageButtonParameter editButtonParameter;

    gdispImage iconScreenshot;
    gdispImage iconPressedScreenshot;
    ImageButtonParameter screenshotButtonParameter;
};
