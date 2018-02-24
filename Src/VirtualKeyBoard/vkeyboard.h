#include "gfx.h"
#include "src/gwin/gwin_keyboard_layout.h"

/* For this keyboard mapping we use:
 * Set 0 = Lowercase letters
 * Set 1 = Uppercase letters (transient)
 * Set 2 = Uppercase letters (locked)
 * Set 3 = Numbers
 * Set 4 = Symbols
 */

static const GVSpecialKey Hex1SKeys[] = { { "\010", "\b", 0, 0 } };

static const char Hex1Set1Row0[] = "123ABC";
static const char Hex1Set1Row1[] = "456DEF";
static const char Hex1Set0Row3[] = "7890";

static const char *Hex1Set0[] = { Hex1Set1Row0, Hex1Set1Row1, Hex1Set0Row3, 0
		, 0 };

static const char *Hex1Set1[] = { 0, 0, 0, 0, 0 };
static const char *Hex1Set2[] = { 0, 0, 0, 0, 0 };
static const char *Hex1Set3[] = { 0, 0, 0, 0, 0, 0 };
static const char *Hex1Set4[] = { 0, 0, 0, 0, 0 };
static const GVKeySet Hex1Sets[] = { Hex1Set0, Hex1Set1, Hex1Set2, Hex1Set3,
		Hex1Set4, 0 };

static const GVKeyTable VirtualKeyboardHexInput = { Hex1SKeys, Hex1Sets };

enum AC_KEYBOARD_TYPE {
	HEX_KEYBOARD,
	ENGLISH_FULL_KEYBOARD
};


void createKeyBoard(AC_KEYBOARD_TYPE);
void deleteKeyBoard();
