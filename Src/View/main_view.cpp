#ifndef __MAIN_VIEW_H
#define __MAIN_VIEW_H

#include "gfx.h"
#include "gwin_table.h"
#include <stdio.h>
#include "tx_can_view.h"
#include "rx_can_view.h"
#include "add_can_message.h"

GListener	gl;
GHandle		ghTabset;
GHandle		tabset_page_1;
GHandle		tabset_page_2;

extern GHandle ghAddButton;
void createTable(void) {
	static bool firstRun = true;
	if(!firstRun) {
		deleteTxCanViewTable();
		deleteRxCanViewTable();
	}
	createTxCanViewTable(&tabset_page_1);
	createRxCanViewTable(&tabset_page_2);
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
	GHandle* ghK;
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
		switch(pe->type) {
			case GEVENT_GWIN_TABSET: {		
				createTable();
				fflush(stdout);
				fflush(stderr);
				break;
			}
			case GEVENT_GWIN_BUTTON: {
				GWindowObject* target = ((GEventGWinButton*)pe)->gwin;
				fprintf(stderr, "%d\n", target);
				fprintf(stderr, "%d", ghAddButton);
				ghK = showAddFrame();
				geventAttachSource(&gl, gwinKeyboardGetEventSource(*ghK), GLISTEN_KEYTRANSITIONS|GLISTEN_KEYUP);
				break;
			}
			case GEVENT_KEYBOARD: {	
				GEventKeyboard * pk = (GEventKeyboard *)pe;
				fprintf(stderr, "%d\n", pk->bytecount);
				if (pk->bytecount) {
					for(uint32_t i = 0; i< pk->bytecount; i++) {
						fprintf(stderr, "%d\n", pk->c[i]);
					}
				}
				fflush(stdout);
				fflush(stderr);
				break;
			}
			case GEVENT_GWIN_SLIDER: {
				setSliderPosition(((GEventGWinSlider *)pe)->position);;
			}
		}
	}
 
	return;
}

#endif



