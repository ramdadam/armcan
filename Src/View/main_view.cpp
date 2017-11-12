#ifndef __MAIN_VIEW_H
#define __MAIN_VIEW_H

#include "gfx.h"
#include "gwin_table.h"
#include <stdio.h>
#include "can_driver.h"
GListener	gl;
GHandle		ghTabset;
GHandle		tabset_page_1;
GHandle		tabset_page_2;
GHandle		table_view;


static font_t titleFont;


#define Gray_40			HTML2COLOR(0x404040)
#define Gray_80			HTML2COLOR(0x808080)
#define Gray_A0			HTML2COLOR(0xA0A0A0)
#define Gray_C0			HTML2COLOR(0xC0C0C0)

void createTable(void) {
	
		GWidgetInit wi;
		gwinWidgetClearInit(&wi);
		wi.g.width = 300;
		wi.g.height = 200;
		wi.g.x = 0;
		wi.g.y = 0;
		wi.g.parent = tabset_page_1;
		wi.g.show = TRUE;
	
		table_view = gwinTableCreate(0, &wi, 3, FALSE);
		//GHandle		tabset_label_page_1;ate(0, &wi, 3, FALSE);
		titleFont = gdispOpenFont("DejaVuSans32");
		#ifndef __cplusplus
		char **header = (char *[]){"Onasdoaisjdoijasdoijsadoie", "Two", "Three"};
		uint32_t** columnWidths = (const uint32_t *[]){60, 100, 140};
		#else
		static char header_temp1[] = {"Onasdoaisjdoijasdoijsadoie"};
		static char header_temp2[] = {"Two"};
		static char header_temp3[] = {"Three"};
		char* header[3] = {header_temp1, header_temp2, header_temp3};
		static uint32_t col1 = 100;
		static uint32_t col2 = 100;
		static uint32_t col3 = 100;
		uint32_t* columnWidths[3] = {&col1, &col2, &col3};
		#endif
		gwinTableSetHeader(table_view, header, &columnWidths[0]);
		gwinTableSetHeaderBackground(table_view, Gray_80);
	
	
		// Add some rows to the first table widget
	/*
		char **values0 = (char *[]){"abc", "bcd", "cde"};
	
		gwinTableAddRow(table_view, values0, TRUE);*/
		gwinTableEnableRender(table_view, TRUE);
}

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

extern "C" void initMainPage(void) {
/*	fprintf(stderr, "\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "\n");
*/	GEvent* pe;
	gfxInit();
	gwinSetDefaultFont(gdispOpenFont("UI2"));
	gwinSetDefaultStyle(&WhiteWidgetStyle, FALSE);
	gdispClear(White);

	createTabset();
	
	// We want to listen for widget events
	geventListenerInit(&gl);
	gwinAttachListener(&gl);
 
	while(1) {
		// Get an Event
		pe = geventEventWait(&gl, TIME_INFINITE);
		if(pe->type == GEVENT_GWIN_TABSET){
			createTable();
			
			
    		fflush(stdout);
			fflush(stderr);
		}
		
   		fflush(stdout);
 		fflush(stderr);
		(void)pe;
	}
 
	return;
}

#endif



