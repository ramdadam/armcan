//
// Created by mohamad on 01.01.19.
//
#include "gfx.h"
#include <Inc/events/event_listener.h>
#include <Inc/View/chat_view.h>
#include "vkeyboard.h"

EVENT_ACTION CChatView::evalEvent(GEvent * gEvent, EVENT_ACTION currentAction) {
    return currentAction;
}

EVENT_ACTION_STATUS CChatView::performAction(EVENT_ACTION action, GEvent * gEvent) {
    return EVENT_HANDLED;
}

CChatView::CChatView() {

}

void CChatView::createChatView(GHandle *parent) {
    font_t font2 = gdispOpenFont("DejaVuSans20");

    GWindowInit		wi;
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
    gwinPutString(ghChatConsole,"Chat ready\n");
    gwinPutString(ghChatConsole,"Chat rea9dy\n");
    gwinPutString(ghChatConsole,"Chat rea468dy\n");
    gwinPutString(ghChatConsole,"Chat read5y\n");
    gwinPutString(ghChatConsole,"Chat rea32dy\n");
    gwinPutString(ghChatConsole,"Chat re4ad4y\n");
    gwinPutString(ghChatConsole,"Chat ready3\n");

    GWidgetInit		wdi;
    gwinWidgetClearInit(&wdi);
    wdi.g.parent = *parent;
    wdi.g.show = TRUE;
    wdi.g.x = 160;
    wdi.g.y = 125;
    wdi.g.width = 300;
    wdi.g.height = 25;
    ghMessageTextEdit = gwinTexteditCreate(nullptr, &wdi, 100);

    gwinWidgetClearInit(&wdi);
    wdi.g.parent = *parent;
    wdi.g.show = TRUE;
    wdi.g.x = 110;
    wdi.g.y = 125;
    wdi.g.height = 25;
    wdi.text = "Msg:";
    ghMessageLabel = gwinLabelCreate(nullptr, &wdi);
    gwinSetFont(ghMessageLabel, font2);
    gwinRedraw(ghMessageLabel);

    gwinWidgetClearInit(&wdi);
    wdi.g.parent = *parent;
    wdi.g.show = TRUE;
    wdi.g.x = 50;
    wdi.g.y = 125;
    wdi.g.width = 50;
    wdi.g.height = 25;
    ghReceiverIDTextEdit = gwinTexteditCreate(nullptr, &wdi, 3);

    gwinWidgetClearInit(&wdi);
    wdi.g.parent = *parent;
    wdi.g.show = TRUE;
    wdi.g.x = 10;
    wdi.g.y = 125;
    wdi.g.height = 25;
    wdi.text = "To:";
    ghReceiverIDLabel = gwinLabelCreate(nullptr, &wdi);
    gwinSetFont(ghReceiverIDLabel, font2);
    gwinRedraw(ghReceiverIDLabel);



//    createKeyBoard(ENGLISH_FULL_KEYBOARD);
//    showKeyBoard();
}

void CChatView::addMessage() {
    static int deimudda = 0;
    gwinPrintf(ghChatConsole,"Chat ready %d\n", deimudda++);
}
