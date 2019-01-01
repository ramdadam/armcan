//
// Created by mohamad on 01.12.18.
//
#include "gfx.h"
#include "event_listener.h"
#include "Inc/View/can_settings_view.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <Inc/View/can_settings_view.h>

#include "WidgetStyles.h"
#include "can_gui_package.h"
#include "can_driver.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

const char *heapTemplate = "Heap: %d of %d bytes free";
const char *stateTemplate = "Current CAN State: %s";
const char *errorCodeTemplate = "Current CAN Error: %s";
const char *prescalerTemplate = "CAN Prescaler: %d";
const char *canSpeedTemplate = "CAN Speed: %d Hz";

const uint16_t heapTextMaxLength = 60;
const uint16_t stateTextMaxLength = 60;
const uint16_t errorTextMaxLength = 100;
const uint16_t prescalerTextMaxLength = 30;
const uint16_t canSpeedTextMaxLength = 40;

CCanSettingsView::CCanSettingsView() {
    canPrescaler = 50;
    canSleepModeActive = false;

    canDriverPrescaler = 50;
    canDriverSleepModeActive = false;
}

void CCanSettingsView::refreshSettings() {
    uint32_t tempCanState = 0;
    uint32_t tempCanErrorCode = 0;

    memset(canStateDescription, 0, stateTextMaxLength);
    memset(canErrorCodeDescription, 0, errorTextMaxLength);
    uint8_t tempCanStateHasError = canDriver.getUserFriendlyState(canStateDescription, &tempCanState);
    uint8_t tempCanErrorCodeIsError = canDriver.getUserFriendlyErrorText(canErrorCodeDescription, &tempCanErrorCode);

    bool errorDidChange =
            tempCanStateHasError != canStateHasError || tempCanErrorCodeIsError != canErrorCodeIsError;

    if (errorDidChange && (tempCanStateHasError || tempCanErrorCodeIsError)) {
        gwinShow(ghReInitCANButton);
        gwinSetStyle(ghCanErrorCodeLabel, &RedTextStyle);
        gwinSetStyle(ghCanStateLabel, &RedTextStyle);
    } else if (errorDidChange) {
        gwinSetStyle(ghCanErrorCodeLabel, &GreenTextStyle);
        gwinSetStyle(ghCanStateLabel, &GreenTextStyle);
    }

    if (tempCanErrorCode != canErrorCode) {
        canErrorCode = tempCanErrorCode;
        canErrorCodeIsError = tempCanErrorCodeIsError;
        snprintf(canErrorCodeLabelText, errorTextMaxLength, errorCodeTemplate, canErrorCodeDescription);
        gwinRedraw(ghCanErrorCodeLabel);
    }

    if (tempCanState != canState) {
        canState = tempCanState;
        canStateHasError = tempCanStateHasError;
        snprintf(canStateLabelText, stateTextMaxLength, stateTemplate, canStateDescription);
        gwinRedraw(ghCanStateLabel);
    }

    size_t currentHeapSize = xPortGetFreeHeapSize();
    if (heapSize != currentHeapSize) {
        snprintf(freeBytesLabelText, heapTextMaxLength, heapTemplate, currentHeapSize, configTOTAL_HEAP_SIZE);
        gwinRedraw(ghFreeBytesLabel);
    }
}

void CCanSettingsView::createSettingsPage(GHandle *parent) {
    this->parent = *parent;
    freeBytesLabelText = (char *) gfxAlloc(sizeof(char) * heapTextMaxLength);
    canStateLabelText = (char *) gfxAlloc(sizeof(char) * stateTextMaxLength);
    canErrorCodeLabelText = (char *) gfxAlloc(sizeof(char) * errorTextMaxLength);
    prescalerLabelText = (char *) gfxAlloc(sizeof(char) * prescalerTextMaxLength);
    canSpeedLabelText = (char *) gfxAlloc(sizeof(char) * canSpeedTextMaxLength);

    canStateDescription = (char *) gfxAlloc(sizeof(char) * stateTextMaxLength);
    canErrorCodeDescription = (char *) gfxAlloc(sizeof(char) * errorTextMaxLength);

    canStateHasError = canDriver.getUserFriendlyState(canStateDescription, &canState);
    canErrorCodeIsError = canDriver.getUserFriendlyErrorText(canErrorCodeDescription, &canErrorCode);

    snprintf(prescalerLabelText, prescalerTextMaxLength, prescalerTemplate, DEFAULT_CAN_PRESCALER);
    snprintf(canSpeedLabelText, canSpeedTextMaxLength, canSpeedTemplate, DEFAULT_CAN_SPEED / DEFAULT_CAN_PRESCALER);


    snprintf(canErrorCodeLabelText, errorTextMaxLength, errorCodeTemplate, canErrorCodeDescription);
    snprintf(canStateLabelText, stateTextMaxLength, stateTemplate, canStateDescription);

    GWidgetInit wi;
    gwinWidgetClearInit(&wi);

    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 20;
    wi.g.parent = *parent;
    wi.text = "Loading Can State";
    ghCanStateLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetStyle(ghCanStateLabel, canErrorCodeIsError ? &RedTextStyle : &GreenTextStyle);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 40;
    wi.g.parent = *parent;
    wi.text = "Loading Can Error State";
    ghCanErrorCodeLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetStyle(ghCanErrorCodeLabel, canStateHasError ? &RedTextStyle : &GreenTextStyle);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 60;
    wi.g.parent = *parent;
    wi.text = "Loading Heap Information";
    ghFreeBytesLabel = gwinLabelCreate(nullptr, &wi);

    gwinWidgetClearInit(&wi);
    wi.g.x = 5;
    wi.g.y = 80;
    wi.g.width = 35;
    wi.g.height = 35;
    wi.g.show = TRUE;
    wi.customDraw = gwinButtonDraw_ArrowDown;
    wi.g.parent = *parent;
    ghPrescalerSubBtn = gwinButtonCreate(nullptr, &wi);

    gwinWidgetClearInit(&wi);
    wi.g.x = 50;
    wi.g.y = 80;
    wi.g.width = 375;
    wi.g.height = 35;
    wi.g.show = TRUE;
    wi.text = "CAN Prescaler (1 - 1024)";
    wi.g.parent = *parent;
    ghPrescalerSlider = gwinSliderCreate(nullptr, &wi);
    gwinSliderSetRange(ghPrescalerSlider, 1, 1024);
    gwinSliderSetPosition(ghPrescalerSlider, DEFAULT_CAN_PRESCALER);

    gwinWidgetClearInit(&wi);
    wi.g.x = 435;
    wi.g.y = 80;
    wi.g.width = 35;
    wi.g.height = 35;
    wi.g.show = TRUE;
    wi.customDraw = gwinButtonDraw_ArrowUp;
    wi.g.parent = *parent;
    ghPrescalerAddBtn = gwinButtonCreate(nullptr, &wi);

    gwinWidgetClearInit(&wi);
    wi.g.x = 5;
    wi.g.y = 125;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.show = TRUE;
    wi.text = "CAN Prescaler";
    wi.g.parent = *parent;
    ghPrescalerLabel = gwinLabelCreate(nullptr, &wi);
    gwinWidgetClearInit(&wi);

    wi.g.x = 5;
    wi.g.y = 145;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.show = TRUE;
    wi.text = "CAN Speed";
    wi.g.parent = *parent;
    ghCanSpeedLabel = gwinLabelCreate(nullptr, &wi);
    gwinWidgetClearInit(&wi);

    wi.g.x = 5;
    wi.g.y = 165;
    wi.g.width = 165;
    wi.g.height = 30;
    wi.g.show = TRUE;
    wi.text = "CAN Sleep Mode: ";
    wi.g.parent = *parent;
    wi.customDraw = gwinCheckboxDraw_CheckOnRight;
    wi.customParam = nullptr;
    wi.customStyle = nullptr;

    ghCanSleepCheckBox = gwinCheckboxCreate(nullptr, &wi);
    gwinCheckboxCheck(ghCanSleepCheckBox, false);
    gwinWidgetClearInit(&wi);


    wi.g.x = 310;
    wi.g.y = 225;
    wi.g.width = 80;
    wi.g.height = 30;
    wi.g.show = FALSE;
    wi.text = "Accept";
    wi.g.parent = *parent;
    ghAcceptChanges = gwinButtonCreate(nullptr, &wi);
    gwinSetStyle(ghAcceptChanges, &GreenButtonStyle);
    gwinWidgetClearInit(&wi);

    wi.g.x = 400;
    wi.g.y = 225;
    wi.g.width = 80;
    wi.g.height = 30;
    wi.g.show = FALSE;
    wi.text = "Cancel";
    wi.g.parent = *parent;
    ghCancelChanges = gwinButtonCreate(nullptr, &wi);
    gwinSetStyle(ghCancelChanges, &RedButtonStyle);
    gwinWidgetClearInit(&wi);

    wi.g.x = 5;
    wi.g.y = 225;
    wi.g.width = 135;
    wi.g.height = 30;
    wi.g.show = FALSE;
    wi.text = "Default Settings";
    wi.g.parent = *parent;
    ghResetToDefaultButton = gwinButtonCreate(nullptr, &wi);
    gwinSetStyle(ghResetToDefaultButton, &GrayButtonStyle);
    gwinWidgetClearInit(&wi);

    wi.g.x = 150;
    wi.g.y = 225;
    wi.g.width = 135;
    wi.g.height = 30;
    wi.g.show = true;
    wi.text = "Re-Init CAN";
    wi.g.parent = *parent;
    ghReInitCANButton = gwinButtonCreate(nullptr, &wi);
    gwinSetStyle(ghReInitCANButton, &GrayButtonStyle);
    gwinWidgetClearInit(&wi);

    font_t font = gdispOpenFont("DejaVuSans16");

    gwinSetFont(ghPrescalerSlider, font);

    gwinSetFont(ghFreeBytesLabel, font);
    gwinSetFont(ghCanStateLabel, font);
    gwinSetFont(ghCanErrorCodeLabel, font);
    gwinSetFont(ghPrescalerLabel, font);
    gwinSetFont(ghCanSpeedLabel, font);

    gwinSetFont(ghCanSleepCheckBox, font);

    gwinSetFont(ghAcceptChanges, font);
    gwinSetFont(ghCancelChanges, font);
    gwinSetFont(ghResetToDefaultButton, font);
    gwinSetFont(ghReInitCANButton, font);


    gwinSetText(ghFreeBytesLabel, freeBytesLabelText, 0);
    gwinSetText(ghCanStateLabel, canStateLabelText, 0);
    gwinSetText(ghCanErrorCodeLabel, canErrorCodeLabelText, 0);
    gwinSetText(ghPrescalerLabel, prescalerLabelText, 0);
    gwinSetText(ghCanSpeedLabel, canSpeedLabelText, 0);
}

EVENT_ACTION CCanSettingsView::evalEvent(GEvent *gEvent, EVENT_ACTION currentAction) {
    switch (gEvent->type) {
        case GEVENT_GWIN_BUTTON: {
            GWindowObject *target = ((GEventGWinButton *) gEvent)->gwin;

            if (target == ghPrescalerAddBtn) {
                return INCREASE_CAN_PRESCALER_BY_ONE;
            } else if (target == ghPrescalerSubBtn) {
                return DECREASE_CAN_PRESCALER_BY_ONE;
            } else if (target == ghAcceptChanges) {
                return ACCEPT_SETTINGS_CHANGE;
            } else if (target == ghCancelChanges) {
                return CANCEL_SETTINGS_CHANGE;
            } else if (target == ghResetToDefaultButton) {
                return RESET_TO_DEFAULT_SETTINGS;
            } else if (target == ghReInitCANButton) {
                return REINIT_CAN;
            }
            break;
        }
        case GEVENT_GWIN_SLIDER: {
            GWindowObject *target = ((GEventGWinSlider *) gEvent)->gwin;
            if (ghPrescalerSlider == target) {
                return CAN_PRESCALER_SLIDER_CHANGE;
            }
        }
        case GEVENT_GWIN_CHECKBOX: {
            GEventGWinCheckbox *checkBoxEvent = (GEventGWinCheckbox *) gEvent;
            GWindowObject *target = checkBoxEvent->gwin;
            if (target == ghCanSleepCheckBox) {
                if (checkBoxEvent->isChecked) {
                    return ACTIVATE_CAN_SLEEP_MODE;
                } else {
                    return DEACTIVATE_CAN_SLEEP_MODE;
                }
            }
        }
        default: {
            return currentAction != NO_ACTION ? currentAction : NO_ACTION;
        }
    }
    return currentAction != NO_ACTION ? currentAction : NO_ACTION;
}
static const uint16_t minPrescalerValue = 1;
static const uint16_t maxPrescalerValue = 1024;

EVENT_ACTION_STATUS CCanSettingsView::performAction(EVENT_ACTION action, GEvent *gEvent) {
    switch (action) {
        case CAN_PRESCALER_SLIDER_CHANGE: {
            canPrescaler = ((GEventGWinSlider *) gEvent)->position;
            onPrescalerSliderChange(canPrescaler);
            break;
        }
        case ACTIVATE_CAN_SLEEP_MODE: {
            onSleepModeChange(true);
            break;
        }
        case DEACTIVATE_CAN_SLEEP_MODE: {
            onSleepModeChange(false);
            break;
        }
        case INCREASE_CAN_PRESCALER_BY_ONE: {
            canPrescaler = MIN(canPrescaler + 1, maxPrescalerValue);
            onPrescalerSliderChange(canPrescaler);
            break;
        }
        case DECREASE_CAN_PRESCALER_BY_ONE: {
            canPrescaler = MAX(canPrescaler - 1, minPrescalerValue);
            onPrescalerSliderChange(canPrescaler);
            break;
        }
        case ACCEPT_SETTINGS_CHANGE: {
            onAcceptButtonClick();
            break;
        }
        case CANCEL_SETTINGS_CHANGE: {
            onCancelButtonClick();
            break;
        }
        case RESET_TO_DEFAULT_SETTINGS: {
            onResetButtonClick();
            break;
        }
        case REINIT_CAN: {
        	gwinDisable(ghReInitCANButton);
            canDriver.MX_CAN1_Init(canPrescaler, canSleepModeActive);
        	gwinEnable(ghReInitCANButton);
            break;
        }
    }
    return EVENT_NOT_HANDLED;
}

void CCanSettingsView::onSleepModeChange(bool sleep) {
    if (sleep != canSleepModeActive) {
        canSleepModeActive = sleep;
        changeResetButtonVisibility();
        changeActionButtonsVisibility(true);
    }
}

void CCanSettingsView::onPrescalerSliderChange(int pos) {
    changeResetButtonVisibility();
    changeActionButtonsVisibility(true);

    snprintf(prescalerLabelText, prescalerTextMaxLength, prescalerTemplate, pos);

    snprintf(canSpeedLabelText, canSpeedTextMaxLength, canSpeedTemplate, DEFAULT_CAN_SPEED / pos);
    gwinRedraw(ghPrescalerLabel);
    gwinRedraw(ghCanSpeedLabel);
}

void CCanSettingsView::onResetButtonClick() {
    canPrescaler = DEFAULT_CAN_PRESCALER;
    canSleepModeActive = false;

    changeResetButtonVisibility();
    changeActionButtonsVisibility(false);

    gwinSliderSetPosition(ghPrescalerSlider, DEFAULT_CAN_PRESCALER);
    gwinCheckboxCheck(ghCanSleepCheckBox, false);
    onPrescalerSliderChange(canPrescaler);
    canDriver.MX_CAN1_Init(canPrescaler, canSleepModeActive);
}

void CCanSettingsView::onAcceptButtonClick() {
    changeActionButtonsVisibility(false);
    changeResetButtonVisibility();
    canDriverPrescaler = canPrescaler;
    canDriverSleepModeActive = canSleepModeActive;
    canDriver.MX_CAN1_Init(canPrescaler, canSleepModeActive);
}

void CCanSettingsView::onCancelButtonClick() {
    canPrescaler = canDriverPrescaler;
    canSleepModeActive = canDriverSleepModeActive;

    gwinSliderSetPosition(ghPrescalerSlider, canPrescaler);
    gwinCheckboxCheck(ghCanSleepCheckBox, canSleepModeActive);
    onPrescalerSliderChange(canPrescaler);
}

void CCanSettingsView::changeActionButtonsVisibility(bool show) {
    if (show && (canDriverPrescaler != canPrescaler || canDriverSleepModeActive != canSleepModeActive)) {
        gwinShow(ghAcceptChanges);
        gwinShow(ghCancelChanges);
    } else {
        gwinHide(ghAcceptChanges);
        gwinHide(ghCancelChanges);
    }
}

void CCanSettingsView::changeResetButtonVisibility() {
    if (canPrescaler != DEFAULT_CAN_PRESCALER || canSleepModeActive) {
        gwinShow(ghResetToDefaultButton);
    } else {
        gwinHide(ghResetToDefaultButton);
    }
}
