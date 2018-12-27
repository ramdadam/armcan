//
// Created by mohamad on 27.12.18.
//
//#include <Inc/ffconf.h>
#include "gfx.h"
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

void CSdSettingsView::createSettingsPage(GHandle *parent) {

    sdDriver.initSdDriver();

    bool isDetected = sdDriver.isSdConnected();
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
    gwinSetDefaultFont(gdispOpenFont("DejaVuSans16"));

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

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 170;
    wi.g.height = 20;
    wi.g.x = 5;
    wi.g.y = 120;
    wi.g.parent = *parent;
    wi.text = "Enable Formatting";
    showFormatBtnCheckBox = gwinCheckboxCreate(nullptr, &wi);
    gwinCheckboxCheck(showFormatBtnCheckBox, false);

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    wi.g.width = 130;
    wi.g.height = 25;
    wi.g.x = 190;
    wi.g.y = 117;
    wi.g.parent = *parent;
    wi.text = "FORMAT SD";
    formatBtn = gwinButtonCreate(nullptr, &wi);
    gwinSetStyle(formatBtn, &RedButtonStyle);

    gwinSetText(isDetectedLabel, isDetectedLabelText, 0);
    gwinSetText(freeSpaceLabel, freeSpaceLabelText, 0);
    gwinSetText(sdStateLabel, sdStateLabelText, 0);
    gwinSetText(sdCardStateLabel, sdCardStateLabelText, 0);
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
