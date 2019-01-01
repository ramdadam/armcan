#include "gfx.h"
#include "can_view.h"
#include "can_gui_package.h"
#include "can_driver.h"
#include "WidgetStyles.h"

GHandle CCanView::createBaseTableWidget(GHandle *parent, uint16_t width, uint16_t height) {
    font_t titleFont = gdispOpenFont("DejaVuSans16");
    enum scroll_t scrollBehaviour = scrollSmooth;
    GWidgetInit wi;

    gwinWidgetClearInit(&wi);
    wi.g.width = width;
    wi.g.height = height;
    wi.g.x = 0;
    wi.g.y = 20;
    wi.g.parent = *parent;
    wi.g.show = TRUE;
    table_view = gwinListCreate(nullptr, &wi, FALSE);

    gwinListSetScroll(table_view, scrollBehaviour);
    gwinSetFont(table_view, titleFont);
    gwinSetStyle(table_view, &WhiteWidgetStyle);

    gwinWidgetClearInit(&wi);
    wi.g.show = false;
    wi.g.width = 0;
    wi.g.height = 15;
    wi.g.parent = *parent;
    wi.g.x = 350;
    wi.g.y = 2;
    wi.text = "Please wait...";
    ghPleaseWaitLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghPleaseWaitLabel, gdispOpenFont("DejaVuSans16"));

    gwinWidgetClearInit(&wi);
    wi.g.show = true;
    wi.g.width = 0;
    wi.g.height = 15;
    wi.g.parent = *parent;
    wi.g.x = 5;
    wi.g.y = 3;
    wi.text = "    CAN";
    ghCanStatusLabel = gwinLabelCreate(nullptr, &wi);
    gwinSetFont(ghCanStatusLabel, gdispOpenFont("DejaVuSans12"));

    gwinSetColor(ghCanStatusLabel, HTML2COLOR(0x28A745));
    gwinFillCircle(ghCanStatusLabel, 7, 6, 5);

    return table_view;
}

void CCanView::redrawTable() {
    gwinRedraw(table_view);
    if(canDriver.hasError()) {
        gwinSetColor(ghCanStatusLabel, HTML2COLOR(0xDC3545));
        gwinFillCircle(ghCanStatusLabel, 7, 6, 5);
    } else {
        gwinSetColor(ghCanStatusLabel, HTML2COLOR(0x28A745));
        gwinFillCircle(ghCanStatusLabel, 7, 6, 5);
    }
}

void CCanView::addRow(char *item) {
    gwinListAddItem(table_view, item, TRUE);
}

void CCanView::deleteTableWidget() {
    if (table_view != NULL) {
        gwinDestroy(table_view);
    }
}

void CCanView::setWaitingLabelVisibility(bool show) {
    gwinSetVisible(ghPleaseWaitLabel, show);
}
