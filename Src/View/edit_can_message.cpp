#include "gfx.h"
#include "can_gui_package.h"
#include "edit_can_message.h"
GTimer** transmitTimers;

void editCanMessage(can_gui_package* package, uint8_t useAlloc = FALSE) {
    GWidgetInit wi;

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    const uint16_t width = 400;
    const uint16_t height = 160;

    gwinSetDefaultFont(gdispOpenFont("DejaVuSans16"));
    wi.g.width = gdispGetWidth();
    wi.g.height = gdispGetHeight();
    wi.g.y = 0;
    wi.g.x = 0;
    ghFrame = gwinContainerCreate(0, &wi, GWIN_CONTAINER_BORDER);

    gwinSetDefaultFont(gdispOpenFont("DejaVuSans24"));
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 25;
    wi.g.height = 35;
    wi.g.parent = ghFrame;
    wi.g.x = 1;
    wi.g.y = 1;
    wi.text = "X";
    ghEditBackButton = gwinButtonCreate(NULL, &wi);
    gwinSetDefaultFont(gdispOpenFont("DejaVuSans16"));

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 35;
    wi.g.height = 35;
    wi.g.parent = ghFrame;
    wi.g.x = 445;
    wi.g.y = 0;
    wi.text = "+";
    ghEditAcceptButton = gwinButtonCreate(NULL, &wi);

}