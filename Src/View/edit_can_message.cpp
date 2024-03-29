#include "gfx.h"
#include "can_gui_package.h"
#include "event_listener.h"
#include "edit_can_message.h"
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "vkeyboard.h"
#include "can_driver.h"

extern gfxQueueGSync *canTransmitQueue;

void sendCANPackageCallback(void *CANPackage) {
    if (CANPackage != 0) {
        //TODO: refactor so that main.cpp takes care of transmission
        canDriver.sendCANPackage(static_cast<can_gui_package *>(CANPackage));
    }
}


EVENT_ACTION CEditMessageView::evalEvent(GEvent *gEvent, EVENT_ACTION currentAction) {

    switch (gEvent->type) {
        case GEVENT_GWIN_BUTTON: {
            GWindowObject *target = ((GEventGWinButton *) gEvent)->gwin;
            if (target == ghEditAcceptButton) {
                return ACCEPT_EDIT;
            } else if (target == ghEditBackButton) {
                return CLOSE_EDIT_VIEW;
            }
            return currentAction != NO_ACTION ? currentAction : NO_ACTION;
        }
        case GEVENT_GWIN_RADIO: {
            if (gwinRadioIsPressed(getCyclicRadioSelect())) {
                return SHOW_CYCLIC_TEXTBOX;
            } else {
                return HIDE_CYCLIC_TEXTBOX;
            }
        }
        default: {
            return currentAction != NO_ACTION ? currentAction : NO_ACTION;
        }
    }
}

EVENT_ACTION_STATUS CEditMessageView::performAction(EVENT_ACTION action, GEvent *gEvent) {
    switch (action) {
        case ACCEPT_EDIT: {
            saveEditForm();
            deleteEditForm();
            break;
        }
        case CLOSE_EDIT_VIEW: {
            deleteEditForm();
            break;
        }
        case SHOW_CYCLIC_TEXTBOX: {
            showCyclicTextbox();
            break;
        }
        case HIDE_CYCLIC_TEXTBOX: {
            hideCyclicTextbox();
            break;
        }

    }
        return EVENT_HANDLED;
}

void CEditMessageView::editCanMessage(can_gui_package *package, uint8_t useAlloc = FALSE) {
    GWidgetInit wi;
    currentPackage = package;

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;

    wi.g.width = gdispGetWidth();
    wi.g.height = gdispGetHeight();
    wi.g.y = 0;
    wi.g.x = 0;
    ghFrame = gwinContainerCreate(nullptr, &wi, GWIN_CONTAINER_BORDER);
    font_t font = gdispOpenFont("DejaVuSans24");
    gwinSetFont(ghFrame, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 25;
    wi.g.height = 35;
    wi.g.parent = ghFrame;
    wi.g.x = 1;
    wi.g.y = 1;
    wi.text = "X";
    ghEditBackButton = gwinButtonCreate(nullptr, &wi);
    gwinSetFont(ghEditBackButton, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 95;
    wi.g.height = 35;
    wi.g.parent = ghFrame;
    wi.g.x = 385;
    wi.g.y = 0;
    wi.text = "Save";
    ghEditAcceptButton = gwinButtonCreate(nullptr, &wi);
    gwinSetFont(ghEditAcceptButton, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.x = 60;
    wi.g.y = 5;
    wi.g.width = 0;
    wi.g.height = 0;
    wi.g.parent = ghFrame;

    char idString[30] = {' '};
    snprintf(idString, 30, "ID(hex): 0x%x", package->id);
    wi.text = idString;
    ghEditIDLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghEditIDLabel, font);
    gwinSetText(ghEditIDLabel, idString, true);

    if (package->isRemote) {
        gwinWidgetClearInit(&wi);
        wi.g.x = 60;
        wi.g.show = TRUE;
        wi.g.y = 40;
        wi.g.width = 180;
        wi.g.height = 30;
        wi.g.parent = ghFrame;
        wi.text = "Remote";
        wi.customDraw = gwinCheckboxDraw_CheckOnRight;
        wi.customParam = 0;
        wi.customStyle = 0;
        ghEditCheckbox = gwinCheckboxCreate(nullptr, &wi);
        gwinSetFont(ghEditCheckbox, font);

        gwinCheckboxCheck(ghEditCheckbox, 1);
        gwinDisable(ghEditCheckbox);
    } else {
        gwinWidgetClearInit(&wi);
        char dlcString[12];
        snprintf(dlcString, 12, "DLC: %d Byte |", package->dlc);

        wi.g.show = TRUE;
        wi.g.x = 60;
        wi.g.y = 40;
        wi.g.width = 0;
        wi.g.height = 0;
        wi.g.parent = ghFrame;
        ghEditDLCLabel = gwinLabelCreate(nullptr, &wi);
        gwinSetFont(ghEditDLCLabel, font);
        gwinSetText(ghEditDLCLabel, dlcString, true);

        // example: D: 00:00:00:00:00:00:00
        char dataString[30];
        for (int i = 0; i < 30; i++) {
            dataString[i] = 0;
        }
        const char *dataDescription = "D: ";
        const char *format = "%02X";
        const char *formatDpp = "%02X:";
        size_t pos = strlen(dataDescription);
        strncat(&dataString[0], dataDescription, strlen(dataDescription));

        for (uint8_t i = 0; i < package->dlc; i++) {
            char buffer[6] = {0};
            if (i == package->dlc - 1) {
                snprintf(buffer, 4, format, package->data.data_b[i]);
                strncat(&dataString[pos], buffer, 2);
                pos += 2;
            } else {
                snprintf(buffer, 5, formatDpp, package->data.data_b[i]);
                strncat(&dataString[pos], buffer, 3);
                pos += 3;
            }
        }

        gwinWidgetClearInit(&wi);
        wi.g.show = TRUE;
        wi.g.x = 60;
        wi.g.y = 75;
        wi.g.width = 470;
        wi.g.height = 30;
        wi.g.parent = ghFrame;
        ghEditDataValue = gwinLabelCreate(nullptr, &wi);
        gwinSetFont(ghEditDataValue, font);
        gwinSetText(ghEditDataValue, dataString, true);

    }

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.customDraw = nullptr;
    wi.customParam = nullptr;
    wi.customStyle = nullptr;
    wi.g.x = 60;
    wi.g.y = 105;
    wi.g.width = 170;
    wi.g.height = 30;
    wi.g.parent = ghFrame;
    wi.text = "Zyklisch";
    ghEditRadioSelectCyclic = gwinRadioCreate(NULL, &wi, 0);
    gwinSetFont(ghEditRadioSelectCyclic, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.customDraw = 0;
    wi.customParam = 0;
    wi.customStyle = 0;
    wi.g.x = 235;
    wi.g.y = 105;
    wi.g.width = 150;
    wi.g.height = 30;
    wi.g.parent = ghFrame;
    wi.text = "Einzeln";
    ghEditRadioSelectOnce = gwinRadioCreate(NULL, &wi, 0);
    gwinSetFont(ghEditRadioSelectOnce, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 0;
    wi.customDraw = 0;
    wi.customParam = 0;
    wi.customStyle = 0;
    wi.g.x = 35;
    wi.g.y = 145;
    wi.g.width = 150;
    wi.g.height = 30;
    wi.g.parent = ghFrame;
    wi.text = "Zyklus: ";
    ghEditCycleLabel = gwinLabelCreate(NULL, &wi);
    gwinSetFont(ghEditCycleLabel, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 0;
    wi.g.x = 190;
    wi.g.y = 145;
    wi.g.width = 150;
    wi.g.height = 30;
    wi.g.parent = ghFrame;
    ghEditCycleTextEdit = gwinTexteditCreate(NULL, &wi, 4);
    gwinSetFont(ghEditCycleTextEdit, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = 0;
    wi.g.x = 345;
    wi.g.y = 145;
    wi.g.width = 0;
    wi.g.height = 0;
    wi.g.parent = ghFrame;
    wi.text = "ms";
    ghEditCycleMsLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghEditCycleMsLabel, font);

    createKeyBoard(NUMERIC_KEYBOARD);

    if (package->timer != nullptr && package->cycle > 0) {
        char buffer[6];
        snprintf(buffer, 6, "%d", package->cycle);
        gwinSetText(ghEditCycleTextEdit, buffer, TRUE);
        gwinRadioPress(ghEditRadioSelectCyclic);
        showCyclicTextbox();
    } else {
        gwinRadioPress(ghEditRadioSelectOnce);
    }

}

void CEditMessageView::hideCyclicTextbox() {
    hideKeyBoard();
    gwinHide(ghEditCycleTextEdit);
    gwinHide(ghEditCycleLabel);
    gwinHide(ghEditCycleMsLabel);
    showCyclicBox = 0;
}

void CEditMessageView::showCyclicTextbox() {
    showKeyBoard();
    gwinShow(ghEditCycleTextEdit);
    gwinShow(ghEditCycleLabel);
    gwinShow(ghEditCycleMsLabel);

    showCyclicBox = 1;
}

void CEditMessageView::saveEditForm() {
    if (showCyclicBox == 1) {
        const char *cycleTimeStr = gwinGetText(ghEditCycleTextEdit);
        currentPackage->cycle = strtoul(cycleTimeStr, nullptr, 10);
        if (currentPackage->timer != nullptr) {
            auto *timer = (GTimer *) currentPackage->timer;
            gtimerJab(timer);
            gtimerStart(timer, sendCANPackageCallback, currentPackage, TRUE, currentPackage->cycle);
        } else {
            auto *timer = (GTimer *) gfxAlloc(sizeof(GTimer));
            gtimerInit(timer);
            gtimerStart(timer, sendCANPackageCallback, currentPackage, TRUE, currentPackage->cycle);
            currentPackage->timer = (void *) timer;
        }
    } else {
        if (currentPackage->timer != nullptr) {
            auto *timer = (GTimer *) currentPackage->timer;
            immediateDeleteTimer(timer);
            currentPackage->timer = nullptr;
            currentPackage->cycle = 0;
        }
    }
}

void CEditMessageView::immediateDeleteTimer(GTimer *timer) {
    if (timer != nullptr) {
        gtimerJab(timer);
        gtimerDeinit(timer);
        gfxFree(timer);
    }
}

void CEditMessageView::deleteEditForm() {
    gwinDestroy(ghFrame);
    ghEditBackButton = nullptr;
    ghEditAcceptButton = nullptr;
    ghEditIDLabel = nullptr;
    ghEditCheckbox = nullptr;
    ghEditDataValue = nullptr;
    ghFrame = 0;
    deleteKeyBoard();
}
