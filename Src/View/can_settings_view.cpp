//
// Created by mohamad on 01.12.18.
//
#include "gfx.h"
#include "event_listener.h"
#include "can_settings_view.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <Inc/can_settings_view.h>

#include "can_gui_package.h"
#include "can_driver.h"

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
}

void CCanSettingsView::updateHeapLabel() {
    uint32_t tempCanState = 0;
    uint32_t tempCanErrorCode = 0;

    uint8_t tempCanStateHasError = canDriver.getUserFriendlyState(canStateLabelText, &tempCanState);
    uint8_t tempCanErrorCodeIsError = canDriver.getUserFriendlyErrorText(canErrorCodeLabelText, &tempCanErrorCode);

    bool updateNeeded = false;

    if(tempCanErrorCode != canErrorCode) {
        canErrorCode = tempCanErrorCode;
        canErrorCodeIsError = tempCanErrorCodeIsError;
        snprintf(canErrorCodeLabelText, errorTextMaxLength, errorCodeTemplate, canErrorCodeDescription);
        updateNeeded = true;
    }
    if(tempCanState != canState) {
        canState = tempCanState;
        canStateHasError = tempCanStateHasError;
        snprintf(canStateLabelText, stateTextMaxLength, stateTemplate, canStateDescription);
        updateNeeded = true;
    }

    size_t currentHeapSize = xPortGetFreeHeapSize();
    if(heapSize != currentHeapSize) {
        snprintf(freeBytesLabelText, heapTextMaxLength, heapTemplate, currentHeapSize, configTOTAL_HEAP_SIZE);
        updateNeeded = true;
    }

    if(updateNeeded) {
//        gwinRedraw(parent);
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

    canStateHasError = canDriver.getUserFriendlyState(canStateLabelText, &canState);
    canErrorCodeIsError = canDriver.getUserFriendlyErrorText(canErrorCodeLabelText, &canErrorCode);

    snprintf(prescalerLabelText, prescalerTextMaxLength, prescalerTemplate, DEFAULT_CAN_PRESCALER);
    snprintf(canSpeedLabelText, canSpeedTextMaxLength, canSpeedTemplate, DEFAULT_CAN_SPEED / DEFAULT_CAN_PRESCALER);


    GWidgetInit wi;
    gwinSetDefaultFont(gdispOpenFont("DejaVuSans16"));
    gwinWidgetClearInit(&wi);

    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 20;
    wi.g.parent = *parent;
    wi.text = "Loading Can State";
    ghCanStateLabel = gwinLabelCreate(nullptr, &wi);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 40;
    wi.g.parent = *parent;
    wi.text = "Loading Can Error State";
    ghCanErrorCodeLabel = gwinLabelCreate(nullptr, &wi);

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
    wi.g.x = 20;
    wi.g.y = 90;
    wi.g.width = 435;
    wi.g.height = 35;
    wi.g.show = TRUE;
    wi.text = "CAN Prescaler (1 - 1024)";
    wi.g.parent = *parent;
    ghPrescalerSlider = gwinSliderCreate(nullptr, &wi);
    gwinSliderSetRange(ghPrescalerSlider, 1, 1024);
    gwinSliderSetPosition(ghPrescalerSlider, DEFAULT_CAN_PRESCALER);


    gwinWidgetClearInit(&wi);
    wi.g.x = 5;
    wi.g.y = 145;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.show = TRUE;
    wi.text = "CAN Prescaler";
    wi.g.parent = *parent;
    ghPrescalerLabel = gwinLabelCreate(nullptr, &wi);
    gwinWidgetClearInit(&wi);

    wi.g.x = 5;
    wi.g.y = 165;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.show = TRUE;
    wi.text = "CAN Speed";
    wi.g.parent = *parent;
    ghCanSpeedLabel = gwinLabelCreate(nullptr, &wi);

    gwinSetText(ghFreeBytesLabel, freeBytesLabelText, 0);
    gwinSetText(ghCanStateLabel, canStateLabelText, 0);
    gwinSetText(ghCanErrorCodeLabel, canErrorCodeLabelText, 0);
    gwinSetText(ghPrescalerLabel, prescalerLabelText, 0);
    gwinSetText(ghCanSpeedLabel, canSpeedLabelText, 0);

    gwinSetDefaultFont(gdispOpenFont("DejaVuSans24"));
}

EVENT_ACTION CCanSettingsView::evalEvent(GEvent * gEvent, EVENT_ACTION currentAction) {
    switch (gEvent->type) {
        case GEVENT_GWIN_BUTTON: {
//            GWindowObject *target = ((GEventGWinButton *) gEvent)->gwin;
//
//            if (target == ghBackButton) {
//                return CLOSE_ADD_VIEW;
//            } else if (target == ghAcceptButton) {
//                return ADD_MESSAGE;
//            }
            break;
        }
        case GEVENT_GWIN_SLIDER: {
            GWindowObject *target = ((GEventGWinSlider *) gEvent)->gwin;
            if(ghPrescalerSlider == target) {
                return CAN_PRESCALER_SLIDER_CHANGE;
            }
        }
        case GEVENT_GWIN_CHECKBOX: {
//            if (gwinCheckboxIsChecked(ghAddIsRemote)) {
//                return ADD_VIEW_SHOW_SLIDER;
//            } else {
//                //TODO: else not working if more than one (global) checkbox event
//                return ADD_VIEW_HIDE_SLIDER;
//            }
        }
        default: {
            return currentAction != NO_ACTION ? currentAction : NO_ACTION;
        }
    }
    return currentAction != NO_ACTION ? currentAction : NO_ACTION;
}

EVENT_ACTION_STATUS CCanSettingsView::performAction(EVENT_ACTION action, GEvent * gEvent) {
    switch(action) {
        case CAN_PRESCALER_SLIDER_CHANGE:
        {
            onPrescalerSliderChange(((GEventGWinSlider *) gEvent)->position);
            break;
        }
    }
    return EVENT_NOT_HANDLED;
}

void CCanSettingsView::onPrescalerSliderChange(int pos) {
    canPrescaler = pos;
    snprintf(prescalerLabelText, prescalerTextMaxLength, prescalerTemplate, pos);
    snprintf(canSpeedLabelText, canSpeedTextMaxLength, canSpeedTemplate, DEFAULT_CAN_SPEED / pos);
//    gwinSetText(ghPrescalerLabel, prescalerLabelText, 0);
//    gwinSetText(ghCanSpeedLabel, canSpeedLabelText, 0);
    gwinRedraw(ghPrescalerLabel);
    gwinRedraw(ghCanSpeedLabel);

}
