
#ifndef __TX_VIEW_H
#define __TX_VIEW_H

#include <ImagePushButton.h>

#define TX_MAX_PACKAGES 25

class CTxCanView : public CCanView, public CEventListener {
public:
    EVENT_ACTION evalEvent(GEvent *, EVENT_ACTION currentAction) override;

    EVENT_ACTION_STATUS performAction(EVENT_ACTION, GEvent *) override;

    GHandle createTxCanViewTable(GHandle *parent);

    can_gui_package *getTxSelectedCANPackage();

    int8_t putTxCanPackage(can_gui_package *package, uint8_t allowPackageDeletion);

    void syncList();

    void createButtonGroup(GHandle *parent);

private:
    void showAllActionButtons();

    void hideAllActionButtons();

    void checkRepeatBtnVisibility(int rowIndex);


    can_gui_package_array txCanContainer = 0;
    uint32_t txCanContainerSize = 0;

    GHandle table;
    GHandle ghAddButton;
    GHandle ghDeleteTXItemButton;
    GHandle ghTxEditButton;
    GHandle ghScreenshotButton;
    GHandle ghRepeatOneButton;

    gdispImage *iconRepeatOne;
    ImageButtonParameter repeatOneButtonParameter;

    gdispImage *iconEdit;
    ImageButtonParameter editButtonParameter;

    gdispImage *iconScreenshot;
    gdispImage *iconPressedScreenshot;
    ImageButtonParameter screenshotButtonParameter;
};

#endif