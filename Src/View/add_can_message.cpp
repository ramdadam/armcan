#include "notification_helper.h"
#include "gfx.h"
#include "event_listener.h"
#include "can_gui_package.h"
#include "Inc/View/add_can_message.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "vkeyboard.h"
#include "ImagePushButton.h"

#include <string.h>
#include "../armcan/ugfx/src/gwin/gwin_class.h"
#include "../armcan/ugfx/src/gwin/gwin_container.h"

EVENT_ACTION CAddCanMessageView::evalEvent(GEvent *gEvent, EVENT_ACTION currentAction) {
    switch (gEvent->type) {
        case GEVENT_GWIN_BUTTON: {
            GWindowObject *target = ((GEventGWinButton *) gEvent)->gwin;

            if (target == ghBackButton) {
                return CLOSE_ADD_VIEW;
            } else if (target == ghAcceptButton) {
                return ADD_MESSAGE;
            }
            break;
        }
        case GEVENT_GWIN_SLIDER: {
            GWindowObject *target = ((GEventGWinSlider *) gEvent)->gwin;
            if(ghSlider1 == target) {
                return ADD_VIEW_SLIDER_CHANGE;
            }
        }
        case GEVENT_GWIN_CHECKBOX: {
            if (gwinCheckboxIsChecked(ghAddIsRemote)) {
                return ADD_VIEW_SHOW_SLIDER;
            } else {
                //TODO: else not working if more than one (global) checkbox event
                return ADD_VIEW_HIDE_SLIDER;
            }
        }
        default: {
            return currentAction != NO_ACTION ? currentAction : NO_ACTION;
        }
    }
    return currentAction != NO_ACTION ? currentAction : NO_ACTION;
}

EVENT_ACTION_STATUS CAddCanMessageView::performAction(EVENT_ACTION action, GEvent * gEvent) {
    switch(action) {
        case CLOSE_ADD_VIEW:
        {
            hideAddFrame();
            break;
        }
        case SHOW_ADD_VIEW:
        {
            showAddFrame();
            break;
        }
        case ADD_MESSAGE: {
            break;
        }
        case ADD_VIEW_SLIDER_CHANGE: {
            setSliderPosition(((GEventGWinSlider *) gEvent)->position);
            break;
        }
        case ADD_VIEW_SHOW_SLIDER: {
            setSliderPosition(0);
            hideSlider();
            break;
        }
        case ADD_VIEW_HIDE_SLIDER:{
            showSlider();
            break;
        }
    }
    return EVENT_NOT_HANDLED;
}

void CAddCanMessageView::showAddFrame() {
    createAddFrame();
    showVirtualKeyboard();
    showKeyBoard();
    byteOrderLabelVisible = 1;
}

void CAddCanMessageView::hideAddFrame() {
    // gwinHide(ghFrame1);
    hideVirtualKeyboard();
    gwinDestroy(ghFrame1);
    ghFrame1 = 0;
    byteOrderLabelVisible = 0;
}

void CAddCanMessageView::createAddFrame() {
    GWidgetInit wi;

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;

    font_t font = gdispOpenFont("DejaVuSans24");
    // Apply the frame parameters
    wi.g.width = gdispGetWidth();
    wi.g.height = gdispGetHeight();
    wi.g.y = 0;
    wi.g.x = 0;
    wi.text = "Add Can Message";
    ghFrame1 = gwinContainerCreate(nullptr, &wi, GWIN_CONTAINER_BORDER);
    gwinSetFont(ghFrame1, font);
    
    gwinWidgetClearInit(&wi);

    wi.g.show = TRUE;
    wi.g.x = 35;
    wi.g.y = 5;
    wi.g.width = 0;
    wi.g.height = 0;
    wi.g.parent = ghFrame1;
    wi.text = "ID(hex)";
    ghLabel1 = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghLabel1, font);

    // TextEdit1
    wi.g.show = TRUE;
    wi.g.x = 165;
    wi.g.y = 5;
    wi.g.width = 265;
    wi.g.height = 35;
    wi.g.parent = ghFrame1;
    wi.text = "";
    ghIDTextEdit = gwinTexteditCreate(0, &wi, 3);
    gwinSetFont(ghIDTextEdit, font);

    // Apply the checkbox parameters
    wi.g.x = 32;
    wi.g.show = TRUE;
    wi.g.y = 50;
    wi.g.width = 180;
    wi.g.height = 30;
    wi.g.parent = ghFrame1;
    wi.text = "Remote";
    wi.customDraw = gwinCheckboxDraw_CheckOnRight;
    wi.customParam = nullptr;
    wi.customStyle = nullptr;
    ghAddIsRemote = gwinCheckboxCreate(nullptr, &wi);
    gwinSetFont(ghAddIsRemote, font);

    gwinWidgetClearInit(&wi);
    wi.g.x = 15;
    wi.g.y = 85;
    wi.g.width = 340;
    wi.g.height = 35;
    wi.g.show = TRUE;
    wi.text = "DLC Length";
    wi.g.parent = ghFrame1;
    ghSlider1 = gwinSliderCreate(nullptr, &wi);
    gwinSliderSetRange(ghSlider1, 0, 8);
    gwinSetFont(ghSlider1, font);

    wi.g.show = TRUE;
    wi.g.x = 365;
    wi.g.y = 85;
    wi.g.width = 120;
    wi.g.height = 35;
    wi.g.parent = ghFrame1;
    wi.text = "0 Byte";
    ghLabel2 = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghLabel2, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 25;
    wi.g.height = 35;
    wi.g.parent = ghFrame1;
    wi.g.x = 1;
    wi.g.y = 1;
    wi.text = "X";
    ghBackButton = gwinButtonCreate(nullptr, &wi);
    gwinSetFont(ghBackButton, font);

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 35;
    wi.g.height = 35;
    wi.g.parent = ghFrame1;
    wi.g.x = 445;
    wi.g.y = 0;
    wi.text = "+";
    ghAcceptButton = gwinButtonCreate(nullptr, &wi);
    gwinSetFont(ghAcceptButton, font);

    createKeyBoard(HEX_KEYBOARD);

    gwinWidgetClearInit(&wi);

    wi.g.show = FALSE;
    wi.g.width = 480;
    wi.g.height = 30;
    wi.g.y = 125;
    wi.g.x = 5;
    wi.g.show = TRUE;
    wi.g.parent = ghFrame1;
    ghTexteditContainer = gwinContainerCreate(nullptr, &wi, 0);

    //MSB and LSB Label
    font_t font12 = gdispOpenFont("DejaVuSans12");
    wi.customDraw = 0;
    wi.customParam = 0;
    wi.customStyle = 0;
    wi.g.show = FALSE;
    wi.g.y = 160;
    wi.g.x = 15;
    wi.g.width = 0;
    wi.g.height = 0;
    wi.text = "MSB";
    ghMSBLabel = gwinLabelCreate(NULL, &wi);
    wi.text = "LSB";
    ghLSBLabel = gwinLabelCreate(NULL, &wi);
    gwinSetFont(ghMSBLabel, font12);
    gwinSetFont(ghLSBLabel, font12);

    font_t font14 = gdispOpenFont("DejaVuSans14");
    for (int i = 0; i < 8; i++) {
        gwinWidgetClearInit(&wi);
        wi.g.show = FALSE;
        wi.g.x = i * 58;
        wi.g.y = 0;
        wi.g.width = 55;
        wi.g.height = 35;
        wi.g.parent = ghTexteditContainer;
        wi.text = "";
        ghDataTextEdits[i] = gwinTexteditCreate(nullptr, &wi, 2);
        gwinSetFont(ghDataTextEdits[i], font14);
    }
}

void CAddCanMessageView::setSliderPosition(int pos) {
    gwinSliderSetPosition(ghSlider1, pos);
    char buffer[33];
    snprintf(buffer, sizeof(buffer), "%d Byte", pos);
    gwinSetText(ghLabel2, buffer, TRUE);

    for (int i = 0; i < 8; i++) {
        if (i == pos - 1 && pos > 1) {
            gwinMove(ghLSBLabel, 15 + i * 58, 160);
        }
        if (i >= pos) {
            gwinSetText(ghDataTextEdits[i], nullptr, 0);
            gwinHide(ghDataTextEdits[i]);
            gwinSetFocus(ghAddIsRemote);
        } else {
            gwinShow(ghDataTextEdits[i]);
            if (pos == 1) {
                gwinSetFocus(ghAddIsRemote);
            }
        }
    }
    if (byteOrderLabelVisible == 1 && pos > 1) {
        gwinShow(ghLSBLabel);
        gwinShow(ghMSBLabel);
        byteOrderLabelVisible = 0;
    }
    if (pos <= 1) {
        gwinHide(ghLSBLabel);
        gwinHide(ghMSBLabel);
        byteOrderLabelVisible = 1;
    }
}

uint8_t CAddCanMessageView::getFormData(can_gui_form_data *formData) {
    const char *idStr = gwinGetText(ghIDTextEdit);
    formData->id = static_cast<uint16_t>(strtoul(idStr, NULL, 16));
    if (formData->id > 0x7FF) {
        showMessage(" ID muss kleiner als 0x7FF sein ");
        return 0;
    }
    formData->dlc = static_cast<uint8_t>(gwinSliderGetPosition(ghSlider1));
    formData->isRemote = static_cast<uint8_t>(gwinCheckboxIsChecked(ghAddIsRemote));
    formData->data.data_l = 0;
    for (uint8_t i = 0; i < formData->dlc; i++) {
        const char *textStr = gwinGetText(ghDataTextEdits[i]);

        if (strlen(textStr) == 0) {
            formData->data.data_b[i] = 0;
            continue;
        }
        if (i < formData->dlc) {
            formData->data.data_b[i] = static_cast<uint8_t>(strtoul(textStr, nullptr, 16));
        } else {
            formData->data.data_b[i] = 0;
        }
        gwinSetText(ghDataTextEdits[i], nullptr, FALSE);
    }

    setSliderPosition(0);
    gwinSliderSetPosition(ghSlider1, 0);
    return 1;
}

void CAddCanMessageView::hideSlider() {
    gwinHide(ghSlider1);
    gwinHide(ghLabel2);
}

void CAddCanMessageView::showSlider() {
    gwinShow(ghSlider1);
    gwinShow(ghLabel2);
}

void CAddCanMessageView::showVirtualKeyboard() {
    createKeyBoard(HEX_KEYBOARD);
}

void CAddCanMessageView::hideVirtualKeyboard() {
    deleteKeyBoard();
}
