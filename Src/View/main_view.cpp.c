#include "gfx.h"
//#include "gfxconf.h"
//#include "can_view.h"
#include <stdio.h>
GListener	gl;
GHandle		ghTabset;
GHandle		tabset_page_1;
GHandle		tabset_page_2;
//GHandle		tabset_label_page_1;
//GHandle		table_view;


static font_t titleFont;


#define Gray_40			HTML2COLOR(0x404040)
#define Gray_80			HTML2COLOR(0x808080)
#define Gray_A0			HTML2COLOR(0xA0A0A0)
#define Gray_C0			HTML2COLOR(0xC0C0C0)

void createTabset(void) {
    GWidgetInit wi;
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;

    wi.g.width = 480;
    wi.g.height = 272;
    wi.g.x = 0;
    wi.g.y = 0;
    ghTabset = gwinTabsetCreate(0, &wi, GWIN_TABSET_BORDER);
    tabset_page_1 = gwinTabsetAddTab(ghTabset, "Receive", FALSE);
    tabset_page_2 = gwinTabsetAddTab(ghTabset, "Transmit", FALSE);
    createTable();
}

void createTable(void) {
    createTxCanViewTable(tabset_page_1);
    
//	GWidgetInit wi;
//	gwinWidgetClearInit(&wi);
//	wi.g.width = 300;
//	wi.g.height = 200;
//	wi.g.x = 0;
//	wi.g.y = 0;
//	wi.g.parent = tabset_page_1;
//	wi.g.show = TRUE;

	//table_view = gwinListCreate(0, &wi, FALSE);
	


	//titleFont = gdispOpenFont("DejaVuSans32");

	//char* title = "Table One";
        //gwinTableSetTitle(table_view, title);
        //gwinTableSetTitleBackground(table_view, Gray_A0);
	//gwinTableSetTitleFont(table_view, titleFont);
	//gwinTableSetTitleJustify(table_view, justifyRight);

//	char **header = (char *[]){"Onasdoaisjdoijasdoijsadoie", "Two", "Three"};
//	uint32_t **columnWidths = (uint32_t *[]){100,100,100};
//	gwinTableSetHeader(table_view, header, columnWidths);
//        gwinTableSetHeaderBackground(table_view, Gray_80);


	// Add some rows to the first table widget
        //char* item;
	//sprintf(item, "Item Nr.: %d", 0);

	//gwinListAddItem(table_view, item, TRUE);
//	gwinTableEnableRender(table_view, TRUE);
}

void initMainPage(void) {
	gfxInit();
	gwinSetDefaultFont(gdispOpenFont("UI2"));
	gwinSetDefaultStyle(&WhiteWidgetStyle, FALSE);
	gdispClear(White);

	createTabset();
	
	// We want to listen for widget events
	geventListenerInit(&gl);
	gwinAttachListener(&gl);
 	GEvent *pe = 0;

	while(1) {
		// Get an Event
		pe = geventEventWait(&gl, TIME_INFINITE);
		if(pe->type == GEVENT_GWIN_TABSET){
			createTable();
			
			fprintf(stderr, "tabset\n");
			
    		fflush(stdout);
			fflush(stderr);
		}
		fprintf(stderr, "gevent\n");
		
   		fflush(stdout);
 		fflush(stderr);
		(void)pe;
	}
 
	return;
}





