//
// Created by mohamad on 01.12.18.
//

#ifndef ARMCAN_CAN_SETTINGS_VIEW_H
#define ARMCAN_CAN_SETTINGS_VIEW_H

const GWidgetStyle GreenButtonStyle = {
        HTML2COLOR(0xFFFFFF),			// window background
        HTML2COLOR(0x2A8FCD),			// focused

// enabled color set
        {
                HTML2COLOR(0xffffff),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0x28A745),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        },

// disabled color set
        {
                HTML2COLOR(0xffffff),		// text
                HTML2COLOR(0x808080),		// edge
                HTML2COLOR(0x28A745),		// fill
                HTML2COLOR(0xC0E0C0)		// progress - active area
        },

// pressed color set
        {
                HTML2COLOR(0xffffff),		// text
                HTML2COLOR(0x1e7e34),		// edge
                HTML2COLOR(0x218838),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        }
};
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

const GWidgetStyle RedButtonStyle = {
        HTML2COLOR(0xFFFFFF),			// window background
        HTML2COLOR(0x2A8FCD),			// focused

// enabled color set
        {
                HTML2COLOR(0xE0E0E0),		// text
                HTML2COLOR(0x404040),		// edge
                HTML2COLOR(0xDC3545),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        },

// disabled color set
        {
                HTML2COLOR(0xE0E0E0),		// text
                HTML2COLOR(0x808080),		// edge
                HTML2COLOR(0xDC3545),		// fill
                HTML2COLOR(0xC0E0C0)		// progress - active area
        },

// pressed color set
        {
                HTML2COLOR(0xE0E0E0),		// text
                HTML2COLOR(0xbd2130),		// edge
                HTML2COLOR(0xc82333),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        }
};
const GWidgetStyle GrayButtonStyle = {
        HTML2COLOR(0xFFFFFF),			// window background
        HTML2COLOR(0x2A8FCD),			// focused

// enabled color set
        {
                HTML2COLOR(0xE0E0E0),		// text
                HTML2COLOR(0x6c757d),		// edge
                HTML2COLOR(0x6c757d),		// fill
                HTML2COLOR(0x00E000)		// progress - active area
        },

// disabled color set
        {
                HTML2COLOR(0xE0E0E0),		// text
                HTML2COLOR(0x808080),		// edge
                HTML2COLOR(0x6c757d),		// fill
                HTML2COLOR(0xC0E0C0)		// progress - active area
        },

// pressed color set
        {
                HTML2COLOR(0xE0E0E0),		// text
                HTML2COLOR(0x545b62),		// edge
                HTML2COLOR(0x5a6268),		// fill
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
