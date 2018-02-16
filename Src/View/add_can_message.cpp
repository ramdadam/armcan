#include "gfx.h"
#include "add_can_message.h"
#include <stdio.h>
#include <stdlib.h>

#include "../armcan/ugfx/src/gwin/gwin_class.h"
#include "../armcan/ugfx/src/gwin/gwin_container.h"
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
	wi.g.y = 10;
	wi.g.width = 45;
	wi.g.height = 15;
    wi.g.parent = ghFrame1;
	wi.text = "ID (hex)";
 
	// Create the actual label
    ghLabel1 = gwinLabelCreate(NULL, &wi);
    
	// TextEdit1
	wi.g.show = TRUE;
	wi.g.x = 105;
	wi.g.y = 10;
	wi.g.width = 45;
	wi.g.height = 15;
    wi.g.parent = ghFrame1;
	wi.text = "";
    ghTextedit1 = gwinTexteditCreate(0, &wi, 100);


	wi.g.show = TRUE;
    
    // Apply the checkbox parameters	
	wi.g.x = 50;
	wi.g.y = 30;
	wi.g.width = 65;
	wi.g.height = 15;
    wi.g.parent = ghFrame1;
    wi.text = "Remote";

    // Create the actual checkbox 
    ghCheckbox1 = gwinCheckboxCreate(NULL, &wi);
    
	wi.g.show = TRUE;
    
    // create Slider1
    wi.g.x = 95; 
    wi.g.y = 60; 
    wi.g.width = 100; 
    wi.g.height = 15; 
    wi.text = "DLC";
    wi.g.parent = ghFrame1;
    ghSlider1 = gwinSliderCreate(NULL, &wi);
    gwinSliderSetRange(ghSlider1, 0, 8);

	wi.g.show = TRUE;
	wi.g.x = 50;
	wi.g.y = 60;
	wi.g.width = 45;
	wi.g.height = 15;
    wi.g.parent = ghFrame1;
	wi.text = "0 Byte";
 
	// Create the actual label
    ghLabel2 = gwinLabelCreate(NULL, &wi);

    return &ghFrame1;
}

void setSliderPosition(int pos) {
    char buffer[33];
    snprintf(buffer, sizeof(buffer), "%d Byte", pos);
	gwinSetText(ghLabel2, buffer, TRUE);

    gwinDestroy(ghTexteditContainer);

    GWidgetInit wi;
    gwinWidgetClearInit(&wi);
    
    wi.g.show = FALSE;
    wi.g.width = 400;
    wi.g.height = 30;
    wi.g.y = 90;
    wi.g.x = 0;
    wi.text = "Container";
    wi.g.show = TRUE;
    wi.g.parent = ghFrame1;
    ghTexteditContainer = gwinContainerCreate(0, &wi, 0);

    for(int i=0; i<pos; i++){
        gwinWidgetClearInit(&wi);
        GHandle temp;
        wi.g.show = TRUE;
        wi.g.x = 50 + i*40;
        wi.g.y = 0;
        wi.g.width = 25;
        wi.g.height = 25;
        wi.g.parent = ghTexteditContainer;
        wi.text = "";
        ghDataTextEdits[i] = gwinTexteditCreate(0, &wi, 100);
    }
}

void showVirtualKeyboard() {

}

void hideVirtualKeyboard() {
    
}