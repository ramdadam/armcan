#include "vkeyboard.h"

#include <stdio.h>
#include <stdlib.h>

static GHandle ghKeyboard = nullptr;

void createKeyBoard(AC_KEYBOARD_TYPE keyBoardType) {
    deleteKeyBoard();
    GWidgetInit wi;
    static font_t font;

    font = gdispOpenFont("DejaVuSans12");

    gwinWidgetClearInit(&wi);

    wi.g.x = 0;
    wi.g.width = gdispGetWidth();
    wi.g.height = gdispGetHeight() / 3;
    wi.g.y = gdispGetHeight() - wi.g.height;

    wi.g.show = FALSE;
    static GVKeyTable keyboardLayout;
    switch (keyBoardType) {
        case HEX_KEYBOARD: {
            keyboardLayout = VirtualKeyboardHexInput;
            break;
        }
        case NUMERIC_KEYBOARD: {
            keyboardLayout = VirtualKeyboardNumInput;
            break;
        }
    }
    ghKeyboard = gwinKeyboardCreate(nullptr, &wi);
    gwinSetFont(ghKeyboard, font);
    gwinSetStyle(ghKeyboard, &WhiteWidgetStyle);
    gwinKeyboardSetLayout(ghKeyboard, &keyboardLayout);
}

void deleteKeyBoard() {
    if (ghKeyboard != nullptr) {
        gwinDestroy(ghKeyboard);
        ghKeyboard = nullptr;
    }
}

void showKeyBoard() {
    gwinShow(ghKeyboard);
}

void hideKeyBoard() {
    gwinHide(ghKeyboard);
}