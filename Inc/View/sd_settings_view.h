//
// Created by mohamad on 27.12.18.
//

#ifndef ARMCAN_SD_SETTINGS_VIEW_H
#define ARMCAN_SD_SETTINGS_VIEW_H

class CSdSettingsView : public CEventListener {
public:
        EVENT_ACTION evalEvent(GEvent*, EVENT_ACTION currentAction);
        EVENT_ACTION_STATUS performAction(EVENT_ACTION, GEvent*);
        CSdSettingsView();
        void createSettingsPage(GHandle *parent);
        void updateSettings();
private:

    GHandle parent;
    GHandle settings;

    GHandle isDetectedLabel;
    GHandle freeSpaceLabel;

    GHandle sdStateLabel;
    GHandle sdCardStateLabel;

    GHandle showFormatBtnCheckBox;
    GHandle formatBtn;
    GHandle formatLabel;

    /* texts, no need to free*/
    char *freeSpaceLabelText;
    char *isDetectedLabelText;
    char *sdStateLabelText;
    char *sdCardStateLabelText;
};

#endif //ARMCAN_SD_SETTINGS_VIEW_H
