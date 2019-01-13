
#ifndef ARMCAN_VKEYBOARD_H
#define ARMCAN_VKEYBOARD_H


enum AC_KEYBOARD_TYPE {
	NUMERIC_KEYBOARD,
	HEX_KEYBOARD,
	ENGLISH_FULL_KEYBOARD
};

class VirtualKeyBoard {
public:
	VirtualKeyBoard();
	VirtualKeyBoard(AC_KEYBOARD_TYPE);
	void showKeyBoard();
	void hideKeyBoard();

	void createKeyBoard(AC_KEYBOARD_TYPE);
	void deleteKeyBoard();
    GHandle ghKeyboard = nullptr;
    font_t font;

};

#endif