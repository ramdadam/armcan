#ifndef __MAIN_VIEW_H
#define __MAIN_VIEW_H

#include "gfx.h"
#include "gwin_table.h"
#include <stdio.h>
#include "tx_can_view.h"

GListener	gl;
GHandle		ghTabset;
GHandle		tabset_page_1;
GHandle		tabset_page_2;


void createTable(void) {
	static bool firstRun = true;
	if(!firstRun) {
		deleteTxCanViewTable();
	}
	createTxCanViewTable(&tabset_page_1);
	firstRun = false;
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
	GEvent* pe;
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



