
static GHandle      ghFrame1;
static GHandle		ghKeyboard;
static GHandle		ghTextedit1;
static GHandle		ghLabel1;
static GHandle		ghLabel2;
static GHandle		ghCheckbox1;
static GHandle		ghSlider1;
static uint16_t     lastPos=0;
static GHandle      ghDataTextEdits[8] = {NULL};
GHandle* showAddFrame();
void setSliderPosition(int pos);