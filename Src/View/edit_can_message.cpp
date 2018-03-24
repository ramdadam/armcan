#include "gfx.h"
#include "can_gui_package.h"
#include "edit_can_message.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "vkeyboard.h"

GTimer **transmitTimers;
GHandle ghEditRadioSelectCyclic;
GHandle ghEditRadioSelectOnce;
GHandle ghEditAcceptButton;
GHandle ghEditBackButton;
can_gui_package *currentPackage = 0;

extern gfxQueueGSync canTransmitQueue;

void sendCANPackageCallback(void *CANPackage)
{
    if(CANPackage != 0) {
        can_gui_package* package = (can_gui_package*) CANPackage;
        // gfxQueueGSyncPut(&canTransmitQueue, package);
    }
}

void editCanMessage(can_gui_package *package, uint8_t useAlloc = FALSE)
{
    GWidgetInit wi;
    currentPackage = package;

    gwinWidgetClearInit(&wi);
    wi.g.show = 1;
    const uint16_t width = 400;
    const uint16_t height = 160;

    gwinSetDefaultFont(gdispOpenFont("DejaVuSans16"));
    wi.g.width = gdispGetWidth();
    wi.g.height = gdispGetHeight();
    wi.g.y = 0;
    wi.g.x = 0;
    ghFrame = gwinContainerCreate(0, &wi, GWIN_CONTAINER_BORDER);

    gwinSetDefaultFont(gdispOpenFont("DejaVuSans24"));
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 25;
    wi.g.height = 35;
    wi.g.parent = ghFrame;
    wi.g.x = 1;
    wi.g.y = 1;
    wi.text = "X";
    ghEditBackButton = gwinButtonCreate(NULL, &wi);
    gwinSetDefaultFont(gdispOpenFont("DejaVuSans16"));

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.width = 95;
    wi.g.height = 35;
    wi.g.parent = ghFrame;
    wi.g.x = 385;
    wi.g.y = 0;
    wi.text = "Save";
    ghEditAcceptButton = gwinButtonCreate(NULL, &wi);

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.g.x = 35;
    wi.g.y = 5;
    wi.g.width = 0;
    wi.g.height = 0;
    wi.g.parent = ghFrame;

    char *idString = (char *)gfxAlloc(sizeof(char) * 30);
    snprintf(idString, 30, "ID(hex): 0x%x", package->id);
    wi.text = (const char *)idString;
    ghEditIDLabel = gwinLabelCreate(NULL, &wi);

    if (package->isRemote)
    {
        gwinWidgetClearInit(&wi);
        wi.g.x = 15;
        wi.g.show = TRUE;
        wi.g.y = 40;
        wi.g.width = 180;
        wi.g.height = 30;
        wi.g.parent = ghFrame;
        wi.text = "Remote";
        wi.customDraw = gwinCheckboxDraw_CheckOnRight;
        wi.customParam = 0;
        wi.customStyle = 0;
        ghEditCheckbox = gwinCheckboxCreate(NULL, &wi);
        gwinCheckboxCheck(ghEditCheckbox, 1);
        gwinDisable(ghEditCheckbox);
    }
    else
    {
        gwinWidgetClearInit(&wi);
        char *dlcString = (char *)gfxAlloc(sizeof(char) * 12);
        snprintf(dlcString, 12, "DLC: %d Byte", package->dlc);
        wi.g.show = TRUE;
        wi.g.x = 35;
        wi.g.y = 40;
        wi.g.width = 0;
        wi.g.height = 0;
        wi.g.parent = ghFrame;
        wi.text = (const char *)dlcString;
        ghEditIDLabel = gwinLabelCreate(NULL, &wi);
        // example: D: 00:00:00:00:00:00:00
        char *dataString = (char *)gfxAlloc(sizeof(char) * 30);
        for (int i = 0; i < 30; i++)
        {
            dataString[i] = 0;
        }
        const char *dataDescription = "D: ";
        const char *format = "%02X";
        const char *formatDpp = "%02X:";
        uint16_t pos = strlen(dataDescription);
        strncat(&dataString[0], dataDescription, strlen(dataDescription));

        for (uint8_t i = 0; i < package->dlc; i++)
        {
            char buffer[6] = {0};
            if (i == package->dlc - 1)
            {
                snprintf(buffer, 4, format, package->data.data_b[i]);
                strncat(&dataString[pos], buffer, 2);
                pos += 2;
            }
            else
            {
                snprintf(buffer, 5, formatDpp, package->data.data_b[i]);
                strncat(&dataString[pos], buffer, 3);
                pos += 3;
            }
        }

        gwinWidgetClearInit(&wi);
        wi.g.show = TRUE;
        wi.g.x = 35;
        wi.g.y = 75;
        wi.g.width = 470;
        wi.g.height = 30;
        wi.g.parent = ghFrame;
        wi.text = (const char *)dataString;
        ghEditDataValue = gwinLabelCreate(NULL, &wi);
    }
    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.customDraw = 0;
    wi.customParam = 0;
    wi.customStyle = 0;
    wi.g.x = 35;
    wi.g.y = 105;
    wi.g.width = 170;
    wi.g.height = 30;
    wi.g.parent = ghFrame;
    wi.text = "Zyklisch";
    ghEditRadioSelectCyclic = gwinRadioCreate(NULL, &wi, 0);

    gwinWidgetClearInit(&wi);
    wi.g.show = TRUE;
    wi.customDraw = 0;
    wi.customParam = 0;
    wi.customStyle = 0;
    wi.g.x = 210;
    wi.g.y = 105;
    wi.g.width = 150;
    wi.g.height = 30;
    wi.g.parent = ghFrame;
    wi.text = "Einzeln";
    ghEditRadioSelectOnce = gwinRadioCreate(NULL, &wi, 0);

    gwinWidgetClearInit(&wi);
    wi.g.show = 0;
    wi.customDraw = 0;
    wi.customParam = 0;
    wi.customStyle = 0;
    wi.g.x = 35;
    wi.g.y = 145;
    wi.g.width = 150;
    wi.g.height = 30;
    wi.g.parent = ghFrame;
    wi.text = "Zyklus: ";
    ghEditCycleLabel = gwinLabelCreate(NULL, &wi);

    gwinWidgetClearInit(&wi);
    wi.g.show = 0;
    wi.g.x = 190;
    wi.g.y = 145;
    wi.g.width = 150;
    wi.g.height = 30;
    wi.g.parent = ghFrame;
    ghEditCycleTextEdit = gwinTexteditCreate(NULL, &wi, 4);

    gwinWidgetClearInit(&wi);
    wi.g.show = 0;
    wi.g.x = 345;
    wi.g.y = 145;
    wi.g.width = 0;
    wi.g.height = 0;
    wi.g.parent = ghFrame;
    wi.text = "ms";
    ghEditCycleMsLabel = gwinLabelCreate(NULL, &wi);

    createKeyBoard(NUMERIC_KEYBOARD);

    if(package->timer != 0 && package->cycle > 0) {
        char buffer[6];
        snprintf(buffer, 6, "%d", package->cycle);
        gwinSetText(ghEditCycleTextEdit, buffer, TRUE);
        gwinRadioPress(ghEditRadioSelectCyclic);
        showCyclicTextbox();
    } else {
        gwinRadioPress(ghEditRadioSelectOnce);
    }
    
}

void hideCyclicTextbox()
{
    hideKeyBoard();
    gwinHide(ghEditCycleTextEdit);
    gwinHide(ghEditCycleLabel);
    gwinHide(ghEditCycleMsLabel);
    showCyclicBox = 0;
}

void showCyclicTextbox()
{
    showKeyBoard();
    gwinShow(ghEditCycleTextEdit);
    gwinShow(ghEditCycleLabel);
    gwinShow(ghEditCycleMsLabel);
    showCyclicBox = 1;
}

void saveEditForm()
{
    if (showCyclicBox == 1)
    {
        const char *cycleTimeStr = gwinGetText(ghEditCycleTextEdit);
        currentPackage->cycle = strtoul(cycleTimeStr, NULL, 10);
        if (currentPackage->timer != 0)
        {
            GTimer* timer = (GTimer*) currentPackage->timer;
            gtimerJab(timer);
            gtimerStart(timer, sendCANPackageCallback, currentPackage, TRUE, currentPackage->cycle);
        }
        else
        {
            GTimer *timer = (GTimer*) gfxAlloc(sizeof(GTimer));
            gtimerInit(timer);
            gtimerStart(timer, sendCANPackageCallback, currentPackage, TRUE, currentPackage->cycle);
            currentPackage->timer = (void *)timer;
        }
    }
    else
    {
        if (currentPackage->timer != 0)
        {
            GTimer* timer = (GTimer*) currentPackage->timer;
            immediateDeleteTimer(timer);
            currentPackage->timer = 0;
        }
    }
}

void immediateDeleteTimer(GTimer* timer) {
    if (timer != 0)
    {
        gtimerJab(timer);
        gtimerDeinit(timer);
        gfxFree(timer);
    }
}

void deleteEditForm()
{
    gwinDestroy(ghFrame);
    ghFrame = 0;
    deleteKeyBoard();
}