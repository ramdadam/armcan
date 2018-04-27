#ifndef __MAIN_VIEW_H
#define __MAIN_VIEW_H

#include "notification_helper.h"
#include "main_view.h"
#include "gfx.h"
#ifdef CAN_HARDWARE_ENABLED
#include "can_driver.h"
#endif
#include "gwin_table.h"
#include "can_gui_package.h"
#include <stdio.h>
#include "tx_can_view.h"
#include "rx_can_view.h"
#include "add_can_message.h"
#include "edit_can_message.h"

// void * operator new(std::size_t n)
// {
//   void * const p = gfxAlloc(n);
//   // handle p == 0
//   return p;
// }

void operator delete(void * p) // or delete(void *, std::size_t)
{
  gfxFree(p);
}

GHandle ghTabset = 0;
GHandle tabset_page_1;
GHandle tabset_page_2;
GHandle table_tx;

extern GHandle ghAddButton;
extern GHandle ghDeleteTXItemButton;
//add_can_message back/close button
extern GHandle ghBackButton;
extern GHandle ghAcceptButton;

extern GHandle ghEditRadioSelectCyclic;
extern GHandle ghEditRadioSelectOnce;
extern GHandle ghEditAcceptButton;
extern GHandle ghEditBackButton;

extern GHandle ghTxEditButton;
extern GHandle ghAddIsRemote;

gfxQueueGSync* canTransmitQueue = 0;
gfxQueueGSync* canReceiveQueue = 0;


void createTable(void)
{
	static bool firstRun = true;
	if (!firstRun)
	{
		deleteTxCanViewTable();
		deleteRxCanViewTable();
	}
	table_tx = createTxCanViewTable(&tabset_page_1);
	createRxCanViewTable(&tabset_page_2);
	firstRun = false;
}

void createTabset(void)
{
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);
	wi.g.show = TRUE;

	wi.g.width = 480;
	wi.g.height = 272;
	wi.g.x = 0;
	wi.g.y = 0;
	ghTabset = gwinTabsetCreate(0, &wi, GWIN_TABSET_BORDER);
	tabset_page_1 = gwinTabsetAddTab(ghTabset, "Transmit", 1);
	tabset_page_2 = gwinTabsetAddTab(ghTabset, "Receive", 1);
	createTable();
}

void showMainpage() {
	gwinShow(ghTabset);
	gwinShow(ghAddButton);
	gwinShow(ghTxEditButton);
}

void hideMainpage() {
	gwinHide(ghTabset);
	gwinHide(ghAddButton);
	gwinHide(ghTxEditButton);
}

void initMainPage(void)
{
	gfxInit();
	volatile GEvent *pe;
	GListener gl;
	gwinSetDefaultFont(gdispOpenFont("DejaVuSans12"));
	gwinSetDefaultStyle(&WhiteWidgetStyle, FALSE);
	gdispClear(White);
	createTabset();
	initNotifications();
	geventListenerInit(&gl);
	gwinAttachListener(&gl);
	
	while (1)
	{
		pe = geventEventWait(&gl, TIME_INFINITE);

		switch (pe->type)
		{
		case GEVENT_GWIN_TABSET:
		{
			//createTable();
			break;
		}
		case GEVENT_GWIN_BUTTON:
		{
			GWindowObject *target = ((GEventGWinButton *)pe)->gwin;
			if (target == ghBackButton)
			{
				hideAddFrame();
				showMainpage();
				break;
			}
			//add can frame -> add button clicked
			else if (target == ghAcceptButton)
			{
				can_gui_form_data formData;
				if(getFormData(&formData) == 0) {
					break;
				}
				can_gui_package* package = convertCANFormDataToGuiPackage(&formData);
				buildStringInCanGuiPackage(package);
				if(putTxCanPackage(package, TRUE) != -1) {
					hideAddFrame();
					showMainpage();
				}
				break;
			} else if(target == ghEditAcceptButton) {
				saveEditForm();
				deleteEditForm();
				showMainpage();
				break;
			} else if(target == ghTxEditButton) {
				can_gui_package* tempCanPackage = getTxSelectedCANPackage();
				if(tempCanPackage != 0) {
					editCanMessage(tempCanPackage, FALSE);
					hideMainpage();
				}				
				break;
				}
			else if(target == ghEditBackButton) {
				deleteEditForm();
				showMainpage();
				break;
			}
			else if(target == ghDeleteTXItemButton) {				
				can_gui_package* tempCanPackage = getTxSelectedCANPackage();
				if(tempCanPackage->timer != 0) {
					immediateDeleteTimer((GTimer*)tempCanPackage->timer);
            		tempCanPackage->timer = 0;
				}
				
				//TODO: delete can package and remove from tx view + sort array and redisplay all list items
			}
			else
			{
				gwinHide(ghTabset);
				gwinHide(ghAddButton);
				showAddFrame();
				break;
			}
		}
		case GEVENT_KEYBOARD:
		{
			GEventKeyboard *pk = (GEventKeyboard *)pe;
			if (pk->bytecount)
			{
				for (uint32_t i = 0; i < pk->bytecount; i++)
				{
					fprintf(stderr, "%#010x\n", pk->c[i]);
				}
			}
			break;
		}
		case GEVENT_GWIN_SLIDER:
		{
			setSliderPosition(((GEventGWinSlider *)pe)->position);
			break;
		}
		case GEVENT_GWIN_RADIO:{
			if(gwinRadioIsPressed(ghEditRadioSelectCyclic)) {
				showCyclicTextbox();
			} else {
				hideCyclicTextbox();
			}
			break;
		}
		case GEVENT_GWIN_CHECKBOX:{
			if(gwinCheckboxIsChecked(ghAddIsRemote)) {
				setSliderPosition(0);
				hideSlider();
			} else {
				showSlider();
			}
			break;
		}
		}
	}

	return;
}

#endif
