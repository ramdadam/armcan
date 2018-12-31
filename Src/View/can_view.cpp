#include "gfx.h"
#include "can_view.h"


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

    return table_view;
}

void CCanView::redrawTable() {
    gwinRedraw(table_view);
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
