#include "gfx.h"

GHandle table_view;
static font_t titleFont;

#define Gray_40			HTML2COLOR(0x404040)
#define Gray_80			HTML2COLOR(0x000000)
#define Gray_A0			HTML2COLOR(0xA0A0A0)
#define Gray_C0			HTML2COLOR(0xC0C0C0)

GHandle createBaseTableWidget(GHandle* parent, uint16_t width, uint16_t height) {
        GWidgetInit wi;        
		gwinWidgetClearInit(&wi);
		wi.g.width = width;
        wi.g.height = height;
		wi.g.x = 0;
        wi.g.y = 20;
        
		wi.g.parent = *parent;
        wi.g.show = TRUE;
              
		titleFont = gdispOpenFont("DejaVuSans24");
        color_t oldDefaultColor = gwinGetDefaultColor();

        enum scroll_t scrollBehaviour = scrollSmooth;
        gwinSetDefaultFont(titleFont);
        gwinSetDefaultStyle(&WhiteWidgetStyle, FALSE);
        gwinSetDefaultBgColor(Gray_80);

        table_view = gwinListCreate(NULL, &wi, FALSE);
        gwinListSetScroll(table_view, scrollBehaviour);

        titleFont = gdispOpenFont("DejaVuSans12");

        gwinSetDefaultBgColor(oldDefaultColor);
        return table_view;
}

void addRow(char* item) {
    gwinListAddItem(table_view, item, TRUE);
}

void deleteTableWidget() {
    if(table_view != NULL) {
        gwinDestroy(table_view);
    }
}
