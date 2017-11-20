#include "gfx.h"
#include "add_can_message.h"
#include <stdio.h>
#include <stdlib.h>

#include "../armcan/ugfx/src/gwin/gwin_class.h"
GHandle* showAddFrame() {
    GWidgetInit wi;    
    // Apply some default values for GWIN
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    const uint16_t width = 400;
    const uint16_t height = 160;
    // Apply the frame parameters    
    wi.g.width = width;
    wi.g.height = height;
    wi.g.y = 5;
    wi.g.x = ((480 - width) / 2);;
    wi.text = "Add Can Message";
    ghFrame1 = gwinFrameCreate(0, &wi, GWIN_FRAME_CLOSE_BTN );


	gwinWidgetClearInit(&wi);


	wi.g.show = TRUE;
	wi.g.x = 50;
	wi.g.y = 30;
	wi.g.width = 45;
	wi.g.height = 15;
	wi.text = "ID (hex)";
 
	// Create the actual label
    ghLabel1 = gwinLabelCreate(NULL, &wi);
    
	// TextEdit1
	wi.g.show = TRUE;
	wi.g.x = 105;
	wi.g.y = 30;
	wi.g.width = 45;
	wi.g.height = 15;
	wi.text = "";
    ghTextedit1 = gwinTexteditCreate(0, &wi, 100);


	wi.g.show = TRUE;
    
    // Apply the checkbox parameters	
	wi.g.x = 50;
	wi.g.y = 60;
	wi.g.width = 65;
	wi.g.height = 15;
    wi.text = "Remote";

    // Create the actual checkbox 
    ghCheckbox1 = gwinCheckboxCreate(NULL, &wi);
    
	wi.g.show = TRUE;
    
    // create Slider1
    wi.g.x = 95; 
    wi.g.y = 90; 
    wi.g.width = 100; 
    wi.g.height = 15; 
    wi.text = "DLC";
    ghSlider1 = gwinSliderCreate(NULL, &wi);
    gwinSliderSetRange(ghSlider1, 0, 8);

	wi.g.show = TRUE;
	wi.g.x = 50;
	wi.g.y = 90;
	wi.g.width = 45;
	wi.g.height = 15;
	wi.text = "0 Byte";
 
	// Create the actual label
    ghLabel2 = gwinLabelCreate(NULL, &wi);

	// Create the keyboard
	wi.g.show = TRUE;
    wi.g.x = 0; 
    wi.g.y = 180;
    wi.g.width = 480;
    wi.g.height = 100;
    ghKeyboard = gwinKeyboardCreate(0, &wi);
    return &ghKeyboard;
}

void setSliderPosition(int pos) {
    char buffer[33];
    GWidgetInit wi;    
    // Apply some default values for GWIN
    snprintf(buffer, sizeof(buffer), "%d Byte", pos);
    gwinLabelSetAttribute(ghLabel2, 100, buffer);
    int16_t tempPos = lastPos - pos;

    fprintf(stderr, "lastPos: %d\n", lastPos);
    fprintf(stderr, "pos: %d\n", pos);
    fprintf(stderr, "tempPos: %d\n", tempPos);
    fflush(stdout);
    fflush(stderr);
    if(tempPos < 0) {
        for(int i = lastPos; i < pos; i++) {
            gwinWidgetClearInit(&wi);
            GHandle temp;
            wi.g.show = TRUE;
            wi.g.x = 50 + i*40;
            wi.g.y = 120;
            wi.g.width = 25;
            wi.g.height = 15;
            wi.text = "";
            ghDataTextEdits[i] = gwinTexteditCreate(0, &wi, 100);
            lastPos += 1;
            fprintf(stderr, "temp %d", lastPos);
        }
    } else if(tempPos > 0) {
        for(int i = lastPos; i>pos; i--) {
            gwinDestroy(ghDataTextEdits[i]);
            gwinRedraw(ghTextedit1);
            gwinRedraw(ghLabel1);
            gwinRedraw(ghLabel2);
            gwinRedraw(ghCheckbox1);
            gwinRedraw(ghSlider1);
            gwinRedraw(ghKeyboard);
            fprintf(stderr, "remp %d", i);
            fflush(stdout);
            fflush(stderr);
            lastPos -= 1;
        }
    }
    for(int i = 0; i<pos; i++) {
        gwinRedraw(ghDataTextEdits[i]);
    }
}