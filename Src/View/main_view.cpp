#ifndef __MAIN_VIEW_H
#define __MAIN_VIEW_H

#include "gfx.h"
#include "gwin_table.h"
#include <stdio.h>
#include "tx_can_view.h"
#include "rx_can_view.h"
#include "add_can_message.h"

GHandle		ghTabset = 0;
GHandle		tabset_page_1;
GHandle		tabset_page_2;
GHandle*		table_tx;
gfxQueueASync*  eventQueue;
extern GHandle ghAddButton;

void createTable(void) {
	static bool firstRun = true;
	if(!firstRun) {
		deleteTxCanViewTable();
		deleteRxCanViewTable();
	}
	table_tx = createTxCanViewTable(&tabset_page_1);
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
    gwinSetDefaultFont(gdispOpenFont("DejaVuSans12"));
	gwinSetDefaultStyle(&WhiteWidgetStyle, FALSE);
	gdispClear(White);
	createAddFrame();
	createTabset();


	gfxQueueASyncInit(eventQueue);

	// We want to listen for widget events
 
	while(1) {
		// Get an Event

		gfxQueueASyncItem * event = gfxQueueASyncGet(eventQueue);
		if(event == 0) {

			fprintf(stderr, "getEvent: %d\n", event);
			fflush(stdout);
			fflush(stderr);
			continue;
		}
		pe = (GEvent*) event;
		//pe = geventEventWait(&gl, TIME_INFINITE);
		fprintf(stderr, "getEvent Done");
		fflush(stdout);
		fflush(stderr);
		fprintf(stderr, "%d--\n", pe);
		fflush(stdout);
		fflush(stderr);
		if(pe == 0) {
			continue;
		}
		switch(pe->type) {
			case GEVENT_GWIN_TABSET: {
				createTable();
				break;
			}
			case GEVENT_GWIN_BUTTON: {
				gwinHide(ghTabset);
				GWindowObject* target = ((GEventGWinButton*)pe)->gwin;
				fprintf(stderr, "win button: %d\n", target);
				fprintf(stderr, "%d", ghAddButton);
				fflush(stdout);
				fflush(stderr);
				showAddFrame();
//				ghK = createAddFrame();
				//geventAttachSource(&gl, gwinKeyboardGetEventSource(*ghK), GLISTEN_KEYTRANSITIONS|GLISTEN_KEYUP);
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
			case GEVENT_GWIN_CLOSE: {
				fprintf(stderr, "\n\nHello\n");
				fflush(stdout);
				fflush(stderr);
				hideVirtualKeyboard();
				fprintf(stderr, "Bye\n\n\n");
				fflush(stdout);
				fflush(stderr);
				hideAddFrame();
				gwinShow(ghTabset);
			}
			case GEVENT_GWIN_SLIDER: {
				setSliderPosition(((GEventGWinSlider *)pe)->position);;
			}
		}

	}
 
	return;
}

#endif



