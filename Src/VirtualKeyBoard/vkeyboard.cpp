#include "gfx.h"
#include "Inc/VirtualKeyBoard/vkeyboard.h"
#include "Inc/VirtualKeyBoard/vkeyboard_layout.h"

VirtualKeyBoard::VirtualKeyBoard(AC_KEYBOARD_TYPE keyBoardType) {
    createKeyBoard(keyBoardType);
}
VirtualKeyBoard::VirtualKeyBoard() {
}

void VirtualKeyBoard::showKeyBoard() {
    gwinShow(ghKeyboard);
}

void VirtualKeyBoard::hideKeyBoard() {
    gwinHide(ghKeyboard);
}

void VirtualKeyBoard::createKeyBoard(AC_KEYBOARD_TYPE keyBoardType) {
    GWidgetInit wi;

    font = gdispOpenFont("DejaVuSans12");

    gwinWidgetClearInit(&wi);

    wi.g.x = 0;
    wi.g.width = gdispGetWidth();
    wi.g.height = gdispGetHeight() / 3;
    wi.g.y = gdispGetHeight() - wi.g.height;

    ghKeyboard = gwinKeyboardCreate(nullptr, &wi);
    gwinSetFont(ghKeyboard, font);
    gwinSetStyle(ghKeyboard, &WhiteWidgetStyle);

    wi.g.show = FALSE;
    switch (keyBoardType) {
        case HEX_KEYBOARD: {
            gwinKeyboardSetLayout(ghKeyboard, &VirtualKeyboardHexInput);
            break;
        }
        case NUMERIC_KEYBOARD: {
            gwinKeyboardSetLayout(ghKeyboard, &VirtualKeyboardNumInput);
            break;
        }
        case ENGLISH_FULL_KEYBOARD: {
            gwinKeyboardSetLayout(ghKeyboard, &VirtualKeyboard_English2);
            break;
        }
    }
}

void VirtualKeyBoard::deleteKeyBoard() {
    if (ghKeyboard != nullptr) {
        gwinDestroy(ghKeyboard);
        ghKeyboard = nullptr;
    }
}
