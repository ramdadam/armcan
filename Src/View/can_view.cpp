
#include "gfx.h"
#include "gwin_table.h"

GHandle table_view = NULL;
static font_t titleFont;

#define Gray_40			HTML2COLOR(0x404040)
#define Gray_80			HTML2COLOR(0x808080)
#define Gray_A0			HTML2COLOR(0xA0A0A0)
#define Gray_C0			HTML2COLOR(0xC0C0C0)

GHandle* createBaseTableWidget(char** header, uint32_t** colWidths, uint32_t colCount, GHandle* parent, uint16_t width, uint16_t height) {
        GWidgetInit wi;        
		gwinWidgetClearInit(&wi);
		wi.g.width = width;
        wi.g.height = height;
		wi.g.x = 0;
        wi.g.y = 0;
        
		wi.g.parent = *parent;
        wi.g.show = TRUE;
        
        table_view = gwinTableCreate(0, &wi, 3, FALSE);        
		titleFont = gdispOpenFont("DejaVuSans32");
        
        gwinTableSetHeader(table_view, header, colWidths);
        gwinTableSetHeaderBackground(table_view, Gray_80);
        return &table_view;
}

void deleteTableWidget() {
    if(table_view != NULL) {
        gwinTableDeleteAll(table_view);
    }
}
