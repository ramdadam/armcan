#include "vkeyboard.h"
#include "src/gwin/gwin_keyboard_layout.c"

#include <stdio.h>
#include <stdlib.h>

static GHandle ghKeyboard = 0;
void createKeyBoard(AC_KEYBOARD_TYPE keyBoardType) {
	GWidgetInit wi;
	static font_t font;

	font = gdispOpenFont("DejaVuSans12");
	gwinSetDefaultFont(font);
	gwinSetDefaultStyle(&WhiteWidgetStyle, FALSE);

	gwinWidgetClearInit(&wi);

	wi.g.x = 0;
	wi.g.width = gdispGetWidth();
	wi.g.height = gdispGetHeight() / 3;
	wi.g.y = gdispGetHeight() - wi.g.height;

	wi.g.show = FALSE;
	static GVKeyTable keyboardLayout;
	switch (keyBoardType) {
	case HEX_KEYBOARD:
		keyboardLayout = VirtualKeyboardHexInput;
	}
	ghKeyboard = gwinKeyboardCreate(0, &wi);
	gwinKeyboardSetLayout(ghKeyboard, &keyboardLayout);
}

void deleteKeyBoard() {
	if(ghKeyboard != 0) {
		gwinDestroy(ghKeyboard);
		ghKeyboard=0;
	}
}

void showKeyBoard() {
	gwinShow(ghKeyboard);
}

void hideKeyBoard() {
	gwinHide(ghKeyboard);
}