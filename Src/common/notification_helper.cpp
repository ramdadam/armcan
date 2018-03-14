
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "notification_helper.h"
GTimer timeoutTimer;

const GWidgetStyle ErrorLabelStyle = {
	HTML2COLOR(0xf2dede),			// window background
	HTML2COLOR(0x2A8FCD),			// focused

	// enabled color set
	{
		HTML2COLOR(0xA94442),		// text
		HTML2COLOR(0xebccd1),		// edge
		HTML2COLOR(0xE0E0E0),		// fill
		HTML2COLOR(0xE0E0E0)		// progress - inactive area
	},

	// disabled color set
	{
		HTML2COLOR(0xC0C0C0),		// text
		HTML2COLOR(0x808080),		// edge
		HTML2COLOR(0xE0E0E0),		// fill
		HTML2COLOR(0xC0E0C0)		// progress - active area
	},

	// pressed color set
	{
		HTML2COLOR(0x404040),		// text
		HTML2COLOR(0x404040),		// edge
		HTML2COLOR(0x808080),		// fill
		HTML2COLOR(0x00E000)		// progress - active area
	}
};

void initNotifications() {
    gtimerInit(&timeoutTimer);
    GWidgetInit		wi;
 
	wi.g.show = FALSE;
 
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	wi.g.y = 20;
	wi.g.x = 10;
	wi.g.width = 470;
	wi.g.height = 40;
	wi.g.parent = 0;

	// Create the actual label
	notificationLabel = gwinLabelCreate(NULL, &wi);
	gwinSetStyle(notificationLabel, &ErrorLabelStyle);
}

void timeoutCallback(void* arg) {
    gwinHide(notificationLabel);
}

void showMessage(const char* text) {
	gwinSetText(notificationLabel, text, TRUE);
	gwinShow(notificationLabel);
    gtimerStart(&timeoutTimer, timeoutCallback, NULL, FALSE, 2500);
}

void showError(const char* text) {

}
