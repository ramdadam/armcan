#include "vkeyboard.h"

#include <stdio.h>
#include <stdlib.h>

GHandle ghKeyboard = 0;
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

	wi.g.show = TRUE;
	GVKeyTable keyboardLayout;
	switch (keyBoardType) {
	case HEX_KEYBOARD:
		keyboardLayout = VirtualKeyboardHexInput;
	}
	fprintf(stderr, "a%d\n", ghKeyboard);
	fflush(stdout);
	fflush(stderr);
	ghKeyboard = gwinKeyboardCreate(0, &wi);
	fprintf(stderr, "b%d\n", ghKeyboard);
	fflush(stdout);
	fflush(stderr);
	gwinKeyboardSetLayout(ghKeyboard, &keyboardLayout);
}

void deleteKeyBoard() {
	fprintf(stderr, "pre deleteKeyBoard: %d\n", ghKeyboard);
	fflush(stdout);
	fflush(stderr);
	if(ghKeyboard != 0) {
		gwinDestroy(ghKeyboard);
		ghKeyboard=0;
		fprintf(stderr, "post deleteKeyBoard: %d\n", ghKeyboard);
		fflush(stdout);
		fflush(stderr);
	}
}
