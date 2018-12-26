#ifndef IMAGE_PUSH_BUTTON_H_
#define IMAGE_PUSH_BUTTON_H_
#include "gfx.h"
enum ImageID {
    SCREENSHOT_PRESSED_IMAGE,
    SCREENSHOT_IMAGE,
    REPEAT_ONE_IMAGE,
    SAVE_IMAGE,
    EDIT_IMAGE,
    TIMER_IMAGE,
    ADD_IMAGE,
    BACK_IMAGE
};

struct ImageButtonParameter {
    gdispImage *iconEnabled = nullptr;
    gdispImage *iconHover = nullptr;
};
void loadImageFromRomFs(gdispImage*, ImageID);
GHandle createImagePushButton(GWidgetInit*, ImageButtonParameter*);

#endif