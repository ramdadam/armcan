#include "gfx.h"
#include "add_can_message.h"
#include <stdio.h>
#include <stdlib.h>
#include "vkeyboard.h"

#include "../armcan/ugfx/src/gwin/gwin_class.h"
#include "../armcan/ugfx/src/gwin/gwin_container.h"

void showAddFrame() {
	gwinShow(ghFrame1);
    showKeyBoard();
}

void hideAddFrame() {
	gwinHide(ghFrame1);
    hideKeyBoard();
}

void createAddFrame() {
    GWidgetInit wi;    

    gwinWidgetClearInit(&wi);
    wi.g.show = FALSE;
    const uint16_t width = 400;
    const uint16_t height = 160;

    gwinSetDefaultFont(gdispOpenFont("DejaVuSans12"));
    // Apply the frame parameters    
    wi.g.width =  gdispGetWidth();
    wi.g.height =  gdispGetHeight();
    wi.g.y = 0;
    wi.g.x = 0;
    wi.text = "Add Can Message";
    ghFrame1 = gwinContainerCreate(0, &wi, GWIN_CONTAINER_BORDER);

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
	wi.g.width = 140;
	wi.g.height = 15;
    wi.g.parent = ghFrame1;
	wi.text = "";
    ghTextedit1 = gwinTexteditCreate(0, &wi, 11);


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
 
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 40;
    wi.g.height = 40;
    wi.g.parent = ghFrame1;
    wi.g.x = 400;
    wi.g.y = 20;
    wi.text = "< BACK";
    ghBackButton = gwinButtonCreate(NULL, &wi);

	// Create the actual label
    ghLabel2 = gwinLabelCreate(NULL, &wi);
    createKeyBoard(HEX_KEYBOARD);
   // return &ghFrame1;
}

void setSliderPosition(int pos) {
    char buffer[33];
    snprintf(buffer, sizeof(buffer), "%d Byte", pos);
	gwinSetText(ghLabel2, buffer, TRUE);

    if(ghTexteditContainer != NULL) {
        gfxFree(ghTexteditContainer);
    }

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
        wi.g.show = TRUE;
        wi.g.x = 10+i*55;
        wi.g.y = 0;
        wi.g.width = 40;
        wi.g.height = 25;
        wi.g.parent = ghTexteditContainer;
        wi.text = "";
        ghDataTextEdits[i] = gwinTexteditCreate(0, &wi, 2);
    }
}

void showVirtualKeyboard() {
	createKeyBoard(HEX_KEYBOARD);
}

void hideVirtualKeyboard() {
	deleteKeyBoard();
}
