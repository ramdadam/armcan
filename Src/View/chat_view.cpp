//
// Created by mohamad on 01.01.19.
//
#include "gfx.h"
#include <Inc/events/event_listener.h>
#include <Inc/View/chat_view.h>
#include <string.h>
#include <Inc/common/can_gui_package.h>
#include <cstdlib>
#include <Inc/Driver/can_driver.h>
#include <Inc/common/notification_helper.h>

#include "Inc/VirtualKeyBoard/vkeyboard.h"

EVENT_ACTION CChatView::evalEvent(GEvent *gEvent, EVENT_ACTION currentAction) {
    switch (gEvent->type) {
        case GEVENT_GWIN_TEXTEDIT: {
            GWindowObject *target = ((GEventGWinButton *) gEvent)->gwin;
            if (target == ghMessageTextEdit) {
                return CHAT_SHOW_TEXT_KEYBOARD;
            } else if (target == ghReceiverIDTextEdit) {
                return CHAT_SHOW_HEX_KEYBOARD;
            }
        }
        case GEVENT_GWIN_TABSET: {
            if (gwinGetVisible(parent) == false) {
                return CHAT_HIDE_KEYBOARDS;
            }
        }
        case GEVENT_GWIN_BUTTON: {
            GWindowObject *target = ((GEventGWinButton *) gEvent)->gwin;
            if (target == ghSendBtn) {
                return CHAT_SEND_MESSAGE;
            }
        }
    }
    return currentAction != NO_ACTION ? currentAction : NO_ACTION;
}

EVENT_ACTION_STATUS CChatView::performAction(EVENT_ACTION action, GEvent *gEvent) {
    switch (action) {
        case CHAT_SHOW_TEXT_KEYBOARD: {
            hexKeyboard->hideKeyBoard();
            textKeyboard->showKeyBoard();
            break;
        }
        case CHAT_SHOW_HEX_KEYBOARD: {
            textKeyboard->hideKeyBoard();
            hexKeyboard->showKeyBoard();
            break;
        }
        case CHAT_HIDE_KEYBOARDS: {
            textKeyboard->hideKeyBoard();
            hexKeyboard->hideKeyBoard();
            break;
        }
        case CHAT_SEND_MESSAGE: {
            sendMessage();
            break;
        }
    }
    return EVENT_HANDLED;
}

CChatView::CChatView() {
}

void CChatView::createChatView(GHandle *parent) {

	hrng.Instance = RNG;
    HAL_RNG_Init(&hrng);
    uint32_t temp = 0;
    HAL_RNG_GenerateRandomNumber(&hrng, &temp);
    myId = static_cast<uint16_t>(temp % 0x7FF);

    hexKeyboard = new VirtualKeyBoard(HEX_KEYBOARD);
    textKeyboard = new VirtualKeyBoard(ENGLISH_FULL_KEYBOARD);

    font_t font2 = gdispOpenFont("DejaVuSans20");
    this->parent = *parent;
    GWindowInit wi;
    wi.parent = *parent;
    wi.show = TRUE;
    wi.x = 0;
    wi.y = 20;
    wi.width = gdispGetWidth();
    wi.height = 100;
    ghChatConsole = gwinConsoleCreate(nullptr, &wi);

    /* Use a special font for GW1 */
    gwinSetFont(ghChatConsole, font2);

    /* Set the fore- and background colors for each console */
    gwinSetColor(ghChatConsole, White);
    gwinSetBgColor(ghChatConsole, Gray);

    /* clear all console windows - to set background */
    gwinPutString(ghChatConsole, "Chat ready\n");
    gwinPutString(ghChatConsole, "\n");
    gwinPutString(ghChatConsole, "\n");
    gwinPutString(ghChatConsole, "\n");

    GWidgetInit wdi;
    gwinWidgetClearInit(&wdi);
    wdi.g.parent = *parent;
    wdi.g.show = TRUE;
    wdi.g.x = 425;
    wdi.g.y = 125;
    wdi.g.width = 50;
    wdi.g.height = 25;
    wdi.text = "SEND";
    ghSendBtn = gwinButtonCreate(nullptr, &wdi);

    gwinWidgetClearInit(&wdi);
    wdi.g.parent = *parent;
    wdi.g.show = TRUE;
    wdi.g.x = 140;
    wdi.g.y = 125;
    wdi.g.width = 280;
    wdi.g.height = 25;
    ghMessageTextEdit = gwinTexteditCreate(nullptr, &wdi, CHAT_MESSAGE_MAX_LENGTH);

    gwinWidgetClearInit(&wdi);
    wdi.g.parent = *parent;
    wdi.g.show = TRUE;
    wdi.g.x = 90;
    wdi.g.y = 125;
    wdi.g.height = 25;
    wdi.text = "Msg:";
    ghMessageLabel = gwinLabelCreate(nullptr, &wdi);
    gwinSetFont(ghMessageLabel, font2);
    gwinRedraw(ghMessageLabel);

    gwinWidgetClearInit(&wdi);
    wdi.g.parent = *parent;
    wdi.g.show = TRUE;
    wdi.g.x = 35;
    wdi.g.y = 125;
    wdi.g.width = 50;
    wdi.g.height = 25;
    ghReceiverIDTextEdit = gwinTexteditCreate(nullptr, &wdi, 3);

    gwinWidgetClearInit(&wdi);
    wdi.g.parent = *parent;
    wdi.g.show = TRUE;
    wdi.g.x = 3;
    wdi.g.y = 125;
    wdi.g.height = 25;
    wdi.text = "To:";
    ghReceiverIDLabel = gwinLabelCreate(nullptr, &wdi);
    gwinSetFont(ghReceiverIDLabel, font2);
    gwinRedraw(ghReceiverIDLabel);

    textKeyboard->showKeyBoard();
}

void CChatView::addMessage() {
//    static int dm = 0;
//    gwinPrintf(ghChatConsole,"Chat ready %d\n", dm++);
}

#define NEWLINE_ASCII_CODE 10
#define NEWLINE_ASCII_CHAR '\n'

void CChatView::receiveCanMessage(can_gui_package *package) {
    if (package->dlc < 3 || package->id != myId) {
        return;
    }
    uint16_t senderId = package->data.data_b[0] | package->data.data_b[1] << 8;
    char msgChar = package->data.data_b[2];


    if (msgChar == NEWLINE_ASCII_CHAR || message.currentPos >= CHAT_MESSAGE_MAX_LENGTH) {
        message.messageBuffer[message.currentPos] = '\0';
        gwinPrintf(ghChatConsole, "%x: %s\n", senderId, message.messageBuffer);
        message.sender = 0;
        message.currentPos = 0;
        memset(message.messageBuffer, ' ', CHAT_MESSAGE_BUFFER_LENGTH);
    } else {
        message.messageBuffer[message.currentPos++] = msgChar;
    }
}

void CChatView::sendMessage() {

    const char *receiverIdText = gwinGetText(ghReceiverIDTextEdit);
    const char *messageText = gwinGetText(ghMessageTextEdit);
    auto receiverId = static_cast<uint16_t>(strtoul(receiverIdText, nullptr, 16));
    size_t messageLength = strlen(messageText);

    if (receiverId > 0x7FF) {
        showMessage(" ID muss kleiner als 0x7FF sein ");
        return;
    }
    if (messageLength == 0 || messageLength > CHAT_MESSAGE_MAX_LENGTH) {
        char notification[60] = {" "};
        snprintf(notification, 60, " Nachrichtenlaenge muss zw. 0 und %d liegen ", CHAT_MESSAGE_MAX_LENGTH);
        showMessage(notification);
        return;
    }

    can_gui_package package;
    package.id = receiverId;
    package.dlc = 3;
    package.data.data_b[0] = static_cast<uint8_t>(myId);
    package.data.data_b[1] = static_cast<uint8_t>(myId >> 8);
    for (uint32_t i = 0; i < messageLength; i++) {
        auto messageChar = (uint8_t) messageText[i];
        package.data.data_b[2] = messageChar;
#ifdef CHAT_LOOPBACK_MODE
        receiveCanMessage(&package);
#else
        canDriver.sendCANPackage(&package);
#endif
    }
    package.data.data_b[2] = NEWLINE_ASCII_CODE;
#ifdef CHAT_LOOPBACK_MODE
    receiveCanMessage(&package);
#else
    canDriver.sendCANPackage(&package);
#endif

}
