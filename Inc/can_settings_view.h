//
// Created by mohamad on 01.12.18.
//

#ifndef ARMCAN_CAN_SETTINGS_VIEW_H
#define ARMCAN_CAN_SETTINGS_VIEW_H


class CCanSettingsView : public CEventListener {
        public:
        CCanSettingsView();
        void createSettingsPage(GHandle *parent);

        void updateHeapLabel();
        EVENT_ACTION evalEvent(GEvent*, EVENT_ACTION currentAction);
        EVENT_ACTION_STATUS performAction(EVENT_ACTION, GEvent*);

        private:

        void onPrescalerSliderChange(int pos);

        GHandle parent;
        GHandle settings;

        GHandle ghPrescalerSlider;
        GHandle ghPrescalerLabel;

        GHandle ghCanSpeedLabel;

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

        /* current values*/
        size_t heapSize;
        uint32_t canState;
        uint8_t canStateHasError;

        uint32_t canErrorCode;
        uint8_t canErrorCodeIsError;

        uint16_t canPrescaler;

};

#endif //ARMCAN_CAN_SETTINGS_VIEW_H
