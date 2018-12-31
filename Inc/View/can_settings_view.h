//
// Created by mohamad on 01.12.18.
//

#ifndef ARMCAN_CAN_SETTINGS_VIEW_H
#define ARMCAN_CAN_SETTINGS_VIEW_H


class CCanSettingsView : public CEventListener {
        public:
        CCanSettingsView();
        void createSettingsPage(GHandle *parent);

        void refreshSettings();
        EVENT_ACTION evalEvent(GEvent*, EVENT_ACTION currentAction);
        EVENT_ACTION_STATUS performAction(EVENT_ACTION, GEvent*);

        private:

        void onPrescalerSliderChange(int pos);
        void onSleepModeChange(bool sleep);

        void onResetButtonClick();
        void onAcceptButtonClick();
        void onCancelButtonClick();

        void changeActionButtonsVisibility(bool show);
        void changeResetButtonVisibility();

        GHandle parent;
        GHandle settings;


        GHandle ghPrescalerAddBtn;
        GHandle ghPrescalerSubBtn;
        GHandle ghPrescalerSlider;
        GHandle ghPrescalerLabel;

        GHandle ghCanSpeedLabel;

        GHandle ghCanSleepCheckBox;

        GHandle ghAcceptChanges;
        GHandle ghCancelChanges;
        GHandle ghResetToDefaultButton;
        GHandle ghReInitCANButton;

        GHandle ghFreeBytesLabel;
        GHandle ghCanStateLabel;
        GHandle ghCanErrorCodeLabel;

        /* texts, no need to free*/
        char *freeBytesLabelText;
        char *canStateLabelText;
        char *canErrorCodeLabelText;
        char *prescalerLabelText;
        char *canSpeedLabelText;

        /* current string values*/
        char *canStateDescription;
        char *canErrorCodeDescription;

        /* current view values*/
        size_t heapSize;
        uint32_t canState;
        uint8_t canStateHasError;

        uint32_t canErrorCode;
        uint8_t canErrorCodeIsError;

        uint16_t canPrescaler = 50;
        bool canSleepModeActive = false;

        /* current driver values */
        uint16_t canDriverPrescaler = 50;
        bool canDriverSleepModeActive = false;
};

#endif //ARMCAN_CAN_SETTINGS_VIEW_H
