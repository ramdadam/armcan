#include "gfx.h"
#include "add_can_message.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "vkeyboard.h"

#include "../armcan/ugfx/src/gwin/gwin_class.h"
#include "../armcan/ugfx/src/gwin/gwin_container.h"
GHandle ghBackButton;
void showAddFrame()
{
    gwinShow(ghFrame1);
    showKeyBoard();
    firstStart=1;
}

void hideAddFrame()
{
    gwinHide(ghFrame1);
    hideKeyBoard();
    firstStart=0;
}

void createAddFrame()
{
    GWidgetInit wi;

    gwinWidgetClearInit(&wi);
    wi.g.show = FALSE;
    const uint16_t width = 400;
    const uint16_t height = 160;

    gwinSetDefaultFont(gdispOpenFont("DejaVuSans16"));
    // Apply the frame parameters
    wi.g.width = gdispGetWidth();
    wi.g.height = gdispGetHeight();
    wi.g.y = 0;
    wi.g.x = 0;
    wi.text = "Add Can Message";
    ghFrame1 = gwinContainerCreate(0, &wi, GWIN_CONTAINER_BORDER);

    gwinWidgetClearInit(&wi);

    wi.g.show = TRUE;
    wi.g.x = 35;
    wi.g.y = 5;
    wi.g.width = 0;
    wi.g.height = 0;
    wi.g.parent = ghFrame1;
    wi.text = "ID(hex)";

    ghLabel1 = gwinLabelCreate(NULL, &wi);
    // TextEdit1
    wi.g.show = TRUE;
    wi.g.x = 165;
    wi.g.y = 5;
    wi.g.width = 265;
    wi.g.height = 35;
    wi.g.parent = ghFrame1;
    wi.text = "";
    ghTextedit1 = gwinTexteditCreate(0, &wi, 11);

    // Apply the checkbox parameters
    wi.g.x = 32;
    wi.g.show = TRUE;
    wi.g.y = 50;
    wi.g.width = 180;
    wi.g.height = 30;
    wi.g.parent = ghFrame1;
    wi.text = "Remote";
    // Apply some default values for GWIN
    wi.customDraw = gwinCheckboxDraw_CheckOnRight;
    wi.customParam = 0;
    wi.customStyle = 0;

    // Create the actual checkbox
    ghCheckbox1 = gwinCheckboxCreate(NULL, &wi);

    gwinWidgetClearInit(&wi);
    wi.g.x = 15;
    wi.g.y = 85;
    wi.g.width = 340;
    wi.g.height = 35;
    wi.g.show = TRUE;
    wi.text = "DLC Length";
    wi.g.parent = ghFrame1;
    ghSlider1 = gwinSliderCreate(NULL, &wi);
    gwinSliderSetRange(ghSlider1, 0, 8);

    wi.g.show = TRUE;
    wi.g.x = 365;
    wi.g.y = 85;
    wi.g.width = 120;
    wi.g.height = 35;
    wi.g.parent = ghFrame1;
    wi.text = "0 Byte";
    ghLabel2 = gwinLabelCreate(NULL, &wi);

    gwinSetDefaultFont(gdispOpenFont("DejaVuSans24"));    
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 25;
    wi.g.height = 35;
    wi.g.parent = ghFrame1;
    wi.g.x = 1;
    wi.g.y = 1;
    wi.text = "X";
    ghBackButton = gwinButtonCreate(NULL, &wi);
    createKeyBoard(HEX_KEYBOARD);
    gwinSetDefaultFont(gdispOpenFont("DejaVuSans16"));    

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 35;
    wi.g.height = 35;
    wi.g.parent = ghFrame1;
    wi.g.x = 445;
    wi.g.y = 0;
    wi.text = "+";
    ghAcceptButton = gwinButtonCreate(NULL, &wi);
    createKeyBoard(HEX_KEYBOARD);

    gwinWidgetClearInit(&wi);

    wi.g.show = FALSE;
    wi.g.width = 480;
    wi.g.height = 30;
    wi.g.y = 125;
    wi.g.x = 5;
    wi.text = "Container";
    wi.g.show = TRUE;
    wi.g.parent = ghFrame1;
    ghTexteditContainer = gwinContainerCreate(0, &wi, 0);
 
    //MSB and LSB Label
    gwinSetDefaultFont(gdispOpenFont("DejaVuSans12"));    
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	wi.g.show = FALSE;
	wi.g.y = 160;
	wi.g.x = 15;
	wi.g.width = 0;
	wi.g.height = 0;
	wi.text = "MSB";
	ghMSBLabel = gwinLabelCreate(NULL, &wi);
	wi.text = "LSB";
	ghLSBLabel = gwinLabelCreate(NULL, &wi);

    gwinSetDefaultFont(gdispOpenFont("DejaVuSans14"));
    for (int i = 0; i < 8; i++)
    {
        gwinWidgetClearInit(&wi);
        wi.g.show = FALSE;
        wi.g.x = i * 58;
        wi.g.y = 0;
        wi.g.width = 53;
        wi.g.height = 35;
        wi.g.parent = ghTexteditContainer;
        wi.text = "";
        ghDataTextEdits[i] = gwinTexteditCreate(0, &wi, 2);
    }
    gwinSetDefaultFont(gdispOpenFont("DejaVuSans12"));
}

void setSliderPosition(int pos)
{
    char buffer[33];
    snprintf(buffer, sizeof(buffer), "%d Byte", pos);
    gwinSetText(ghLabel2, buffer, TRUE);

    fprintf(stderr, "pos: %d\n", pos);
    fflush(stderr);
    fflush(stdout);
    for (int i = 0; i < 8; i++)
    {
    fprintf(stderr, "i: %d\n", i);
    fflush(stderr);
    fflush(stdout);
        if(i==0 && pos > 1) {
            // gdispDrawString( 10 + i * 55 , 145, "MSB", gdispOpenFont("DejaVuSans12"), Black);
            // gwinMove(ghMSBLabel, 20+i*55, 155);
        } else if(i==pos-1 && pos > 1) {
            gwinMove(ghLSBLabel, 15+i*58, 160);
        }
        if (i >= pos)
        {
            gwinHide(ghDataTextEdits[i]);
        }
        else
        {
            gwinShow(ghDataTextEdits[i]);
        }
    }
    if(firstStart == 1 && pos > 1) {
        gwinShow(ghLSBLabel);
        gwinShow(ghMSBLabel);
        firstStart = 0;
    }
    if(pos <= 1) {
        gwinHide(ghLSBLabel);
        gwinHide(ghMSBLabel);
        firstStart = 1;
    }
}

void showVirtualKeyboard()
{
    createKeyBoard(HEX_KEYBOARD);
}

void hideVirtualKeyboard()
{
    deleteKeyBoard();
}
