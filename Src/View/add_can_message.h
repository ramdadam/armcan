#ifndef ADDCANMESSAGE_H_
#define ADDCANMESSAGE_H_
static GHandle      ghFrame1;
static GHandle		ghKeyboard;
static GHandle		ghTextedit1;
static GHandle		ghLabel1;
static GHandle		ghLabel2;
static GHandle		ghCheckbox1;
static GHandle		ghMSBLabel;
static GHandle		ghLSBLabel;
static GHandle		ghSlider1;
static uint16_t     lastPos=0;
static GHandle		ghTexteditContainer = NULL;
static GHandle      ghDataTextEdits[8] = {NULL};
extern GHandle      ghBackButton;
static GHandle      ghAcceptButton;
static bool_t       firstStart=0;

void showAddFrame();
void hideAddFrame();

void setSliderPosition(int pos);
void createAddFrame();

void showVirtualKeyboard();
void hideVirtualKeyboard();


#endif