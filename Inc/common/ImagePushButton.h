#ifndef IMAGE_PUSH_BUTTON_H_
#define IMAGE_PUSH_BUTTON_H_
enum ImageID {
    SAVE_IMAGE,
    EDIT_IMAGE,
    TIMER_IMAGE,
    ADD_IMAGE,
    BACK_IMAGE
};
GHandle createImagePushButton(GWidgetInit*,gdispImage*, ImageID);

#endif