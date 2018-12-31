//
// Created by mohamad on 27.12.18.
//
//#include <Inc/ffconf.h>
#include "gfx.h"
#include "event_listener.h"

#include <Inc/common/WidgetStyles.h>
#include <Inc/View/sd_settings_view.h>

#include "sd_settings_view.h"
#include "sd_driver.h"

extern CSdDriver sdDriver;

const char *cardDetectedTemplate = "SD Card%s detected";
const char *freeSpaceTemplate = "Space: %d / %d KiB free";

const char *sdStateTemplate = "SD State: %s";
const char *sdCardStateTemplate = "Sd Card State: %s";

const uint16_t cardDetectedMaxLength = 40;
const uint16_t freeSpaceMaxLength = 40;

const uint16_t sdStateMaxLength = 60;
const uint16_t sdCardStateMaxLength = 60;

CSdSettingsView::CSdSettingsView() {

}

EVENT_ACTION CSdSettingsView::evalEvent(GEvent * gEvent, EVENT_ACTION currentAction) {
    switch (gEvent->type) {
        case GEVENT_GWIN_BUTTON: {
            GWindowObject *target = ((GEventGWinButton *) gEvent)->gwin;

            if (target == formatBtn) {
                return FORMAT_SD;
            }
            break;
        }
        case GEVENT_GWIN_CHECKBOX: {
            GEventGWinCheckbox *checkBoxEvent = (GEventGWinCheckbox *) gEvent;
            GWindowObject *target = checkBoxEvent->gwin;
            if (target == showFormatBtnCheckBox) {
                if (checkBoxEvent->isChecked) {
                    return SHOW_FORMAT_BUTTON;
                } else {
                    return HIDE_FORMAT_BUTTON;
                }
            }
            break;
        }
        default: {
            return currentAction != NO_ACTION ? currentAction : NO_ACTION;
        }
    }
    return currentAction != NO_ACTION ? currentAction : NO_ACTION;
}

EVENT_ACTION_STATUS CSdSettingsView::performAction(EVENT_ACTION action, GEvent * gEvent) {
    switch (action) {
        case SHOW_FORMAT_BUTTON: {
            gwinShow(formatBtn);
            break;
        }
        case HIDE_FORMAT_BUTTON: {
            gwinHide(formatBtn);
            break;
        }
        case FORMAT_SD: {
            gwinShow(formatLabel);
            gwinHide(formatBtn);
            sdDriver.formatFAT32();
            gwinHide(formatLabel);
        }
    }
    return EVENT_NOT_HANDLED;
}

void CSdSettingsView::createSettingsPage(GHandle *parent) {

	bool isDetected = sdDriver.isSdConnected();
    isDetected && sdDriver.initSdDriver();
	isDetected && sdDriver.mountSDCard();

    this->parent = *parent;
    freeSpaceLabelText = (char *) gfxAlloc(sizeof(char) * cardDetectedMaxLength);
    isDetectedLabelText = (char *) gfxAlloc(sizeof(char) * freeSpaceMaxLength);

    sdStateLabelText = (char *) gfxAlloc(sizeof(char) * cardDetectedMaxLength);
    sdCardStateLabelText = (char *) gfxAlloc(sizeof(char) * freeSpaceMaxLength);

    uint32_t freeSpace = isDetected ? sdDriver.getSDFreeSpace() : 0;
    uint32_t totalSpace = isDetected ? sdDriver.getSDTotalSpace() : 0;

    char tempSdStateErrorText[40] = {" "};
    uint32_t tempSdStateErrorCode;
    uint32_t sdStateIsError = sdDriver.getSDState(&tempSdStateErrorText[0], &tempSdStateErrorCode);

    char tempSdCardStateErrorText[40] = {" "};
    uint32_t tempSdCardStateErrorCode;
    uint32_t sdCardStateIsError = sdDriver.getSDCardState(&tempSdCardStateErrorText[0], &tempSdCardStateErrorCode);


    snprintf(isDetectedLabelText, cardDetectedMaxLength, cardDetectedTemplate, isDetected ? "" : " not");
    snprintf(freeSpaceLabelText, freeSpaceMaxLength, freeSpaceTemplate, freeSpace, totalSpace);

    snprintf(sdStateLabelText, sdStateMaxLength, sdStateTemplate, tempSdStateErrorText);
    snprintf(sdCardStateLabelText, sdCardStateMaxLength, sdCardStateTemplate, tempSdCardStateErrorText);

    GWidgetInit wi;
    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 20;
    wi.g.parent = *parent;
    wi.text = isDetectedLabelText;
    isDetectedLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetStyle(isDetectedLabel, !isDetected ? &RedTextStyle : &GreenTextStyle);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 40;
    wi.g.parent = *parent;
    wi.text = freeSpaceLabelText;
    freeSpaceLabel = gwinLabelCreate(nullptr, &wi);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 60;
    wi.g.parent = *parent;
    wi.text = sdStateLabelText;
    sdStateLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetStyle(sdStateLabel, sdStateIsError != 0  ? &RedTextStyle : &GreenTextStyle);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 0;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 80;
    wi.g.parent = *parent;
    wi.text = sdCardStateLabelText;
    sdCardStateLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetStyle(sdCardStateLabel, sdCardStateIsError != 0 ? &RedTextStyle : &GreenTextStyle);
//
//    gwinWidgetClearInit(&wi);
//    wi.g.show = 1;
//    wi.g.width = 170;
//    wi.g.height = 20;
//    wi.g.x = 5;
//    wi.g.y = 120;
//    wi.g.parent = *parent;
//    wi.text = "Enable Formatting";
//    showFormatBtnCheckBox = gwinCheckboxCreate(nullptr, &wi);
//    gwinCheckboxCheck(showFormatBtnCheckBox, false);

//    gwinWidgetClearInit(&wi);
//    wi.g.show = false;
//    wi.g.width = 130;
//    wi.g.height = 25;
//    wi.g.x = 190;
//    wi.g.y = 117;
//    wi.g.parent = *parent;
//    wi.text = "FORMAT SD";
//    formatBtn = gwinButtonCreate(nullptr, &wi);
//    gwinSetStyle(formatBtn, &RedButtonStyle);
//
//    gwinWidgetClearInit(&wi);
//    wi.g.show = false;
//    wi.g.width = 0;
//    wi.g.height = 25;
//    wi.g.x = 190;
//    wi.g.y = 117;
//    wi.g.parent = *parent;
//    wi.text = "Formatting...";
//    formatLabel = gwinLabelCreate(nullptr, &wi);
//    gwinSetStyle(formatLabel, &RedButtonStyle);

    font_t font = gdispOpenFont("DejaVuSans16");
    gwinSetFont(isDetectedLabel, font);
    gwinSetFont(freeSpaceLabel, font);
    gwinSetFont(sdStateLabel, font);
    gwinSetFont(sdCardStateLabel, font);

    gwinSetText(isDetectedLabel, isDetectedLabelText, 0);
    gwinSetText(freeSpaceLabel, freeSpaceLabelText, 0);
    gwinSetText(sdStateLabel, sdStateLabelText, 0);
    gwinSetText(sdCardStateLabel, sdCardStateLabelText, 0);

    uint32_t imageCounter = sdDriver.getLatestScreenshotNumber();
    sdDriver.setImageCounter(++imageCounter);
}

void CSdSettingsView::updateSettings() {

    bool isDetected = sdDriver.isSdConnected();
	if(isDetected) {
		sdDriver.initSdDriver();
		sdDriver.mountSDCard();
	} else {
		sdDriver.unmountSDCard();
		sdDriver.deInitSdDriver();
	}

    uint32_t freeSpace = isDetected ? sdDriver.getSDFreeSpace() : 0;
    uint32_t totalSpace = isDetected ? sdDriver.getSDTotalSpace() : 0;

    char tempSdStateErrorText[40] = {" "};
    uint32_t tempSdStateErrorCode;
    uint32_t sdStateIsError = sdDriver.getSDState(&tempSdStateErrorText[0], &tempSdStateErrorCode);

    char tempSdCardStateErrorText[40] = {" "};
    uint32_t tempSdCardStateErrorCode;
    uint32_t sdCardStateIsError = sdDriver.getSDCardState(&tempSdCardStateErrorText[0], &tempSdCardStateErrorCode);

    snprintf(isDetectedLabelText, cardDetectedMaxLength, cardDetectedTemplate, isDetected ? "" : " not");
    snprintf(freeSpaceLabelText, freeSpaceMaxLength, freeSpaceTemplate, freeSpace, totalSpace);

    snprintf(sdStateLabelText, sdStateMaxLength, sdStateTemplate, tempSdStateErrorText);
    snprintf(sdCardStateLabelText, sdCardStateMaxLength, sdCardStateTemplate, tempSdCardStateErrorText);

    gwinSetStyle(isDetectedLabel, !isDetected ? &RedTextStyle : &GreenTextStyle);
    gwinSetStyle(sdStateLabel, sdStateIsError ? &RedTextStyle : &GreenTextStyle);
    gwinSetStyle(sdCardStateLabel, sdCardStateIsError ? &RedTextStyle : &GreenTextStyle);

    gwinRedraw(isDetectedLabel);
    gwinRedraw(freeSpaceLabel);
    gwinRedraw(sdStateLabel);
    gwinRedraw(sdCardStateLabel);
}
