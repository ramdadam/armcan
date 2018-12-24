//
// Created by mohamad on 01.12.18.
//

#ifndef ARMCAN_CAN_SETTINGS_VIEW_H
#define ARMCAN_CAN_SETTINGS_VIEW_H

const GWidgetStyle GreenTextStyle = {
        HTML2COLOR(0xFFFFFF),			// window background
        HTML2COLOR(0x2A8FCD),			// focused

// enabled color set
        {
                HTML2COLOR(0x28A745),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0xE0E0E0),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        },

// disabled color set
        {
                HTML2COLOR(0x28A745),		// text
                HTML2COLOR(0x808080),		// edge
                HTML2COLOR(0xE0E0E0),		// fill
                HTML2COLOR(0xC0E0C0)		// progress - active area
        },

// pressed color set
        {
                HTML2COLOR(0x28A745),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0x808080),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        }
};
const GWidgetStyle RedTextStyle = {
        HTML2COLOR(0xFFFFFF),			// window background
        HTML2COLOR(0x2A8FCD),			// focused

// enabled color set
        {
                HTML2COLOR(0xDC3545),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0xE0E0E0),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        },

// disabled color set
        {
                HTML2COLOR(0xDC3545),		// text
                HTML2COLOR(0x808080),		// edge
                HTML2COLOR(0xE0E0E0),		// fill
                HTML2COLOR(0xC0E0C0)		// progress - active area
        },

// pressed color set
        {
                HTML2COLOR(0xDC3545),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0x808080),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        }
};
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
