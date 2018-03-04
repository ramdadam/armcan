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
	tabset_page_1 = gwinTabsetAddTab(ghTabset, "Receive", 1);
	tabset_page_2 = gwinTabsetAddTab(ghTabset, "Transmit", 1);
	createTable();
}

extern "C" void initMainPage(void) {
	GEvent* pe;
	GListener	gl;
	gfxInit();
    gwinSetDefaultFont(gdispOpenFont("DejaVuSans12"));
	gwinSetDefaultStyle(&WhiteWidgetStyle, FALSE);
	gdispClear(White);
	createAddFrame();
	createTabset();


	geventListenerInit(&gl);
	gwinAttachListener(&gl);

 
	while(1) {
		
		pe = geventEventWait(&gl, TIME_INFINITE);

		switch(pe->type) {
			case GEVENT_GWIN_TABSET: {
				//createTable();
				break;
			}
			case GEVENT_GWIN_BUTTON: {
				fprintf(stderr, "temp");
				fflush(stderr);
				fflush(stdout);
				gwinHide(ghTabset);
				//GWindowObject* target = ((GEventGWinButton*)pe)->gwin;
				showAddFrame();
				break;
			}
			case GEVENT_KEYBOARD: {	
				GEventKeyboard * pk = (GEventKeyboard *)pe;
				if (pk->bytecount) {
					for(uint32_t i = 0; i< pk->bytecount; i++) {
						fprintf(stderr, "%d\n", pk->c[i]);
					}
				}
				break;
			}
			case GEVENT_GWIN_CLOSE: {
				gwinShow(ghTabset);
				hideVirtualKeyboard();
				hideAddFrame();
				break;
			}
			case GEVENT_GWIN_SLIDER: {
				setSliderPosition(((GEventGWinSlider *)pe)->position);
				break;
			}
		}

	}
 
	return;
}

#endif



