#include "gfx.h"
#include "src/gwin/gwin_keyboard_layout.h"

#include "vkeyboard.h"

/** HEX KEYBOARD **/
static const GVSpecialKey Hex1SKeys[] = {{ "\010", "\b", 0, 0 } };

static const char Hex1Set1Row0[] = "123ABC";
static const char Hex1Set1Row1[] = "456DEF";
static const char Hex1Set0Row3[] = "7890\001";

static const char *Hex1Set0[] = { Hex1Set1Row0, Hex1Set1Row1, Hex1Set0Row3, 0
        , 0 };

static const char *Hex1Set1[] = { 0, 0, 0, 0, 0 };
static const char *Hex1Set2[] = { 0, 0, 0, 0, 0 };
static const char *Hex1Set3[] = { 0, 0, 0, 0, 0, 0 };
static const char *Hex1Set4[] = { 0, 0, 0, 0, 0 };
static const GVKeySet Hex1Sets[] = { Hex1Set0, Hex1Set1, Hex1Set2, Hex1Set3,
                                     Hex1Set4, 0 };

static const GVKeyTable VirtualKeyboardHexInput = { Hex1SKeys, Hex1Sets };

/** NUMERIC KEYBOARD **/
static const GVSpecialKey Num1SKeys[] = {{ "\010", "\b", 0, 0 } };

static const char Num1Set1Row0[] = "1234";
static const char Num1Set1Row1[] = "5678";
static const char Num1Set0Row3[] = "90\001";

static const char *Num1Set0[] = { Num1Set1Row0, Num1Set1Row1, Num1Set0Row3, 0
        , 0 };

static const char *Num1Set1[] = { 0, 0, 0, 0, 0 };
static const char *Num1Set2[] = { 0, 0, 0, 0, 0 };
static const char *Num1Set3[] = { 0, 0, 0, 0, 0, 0 };
static const char *Num1Set4[] = { 0, 0, 0, 0, 0 };
static const GVKeySet Num1Sets[] = { Num1Set0, Num1Set1, Num1Set2, Num1Set3,
                                     Num1Set4, 0 };

static const GVKeyTable VirtualKeyboardNumInput = { Num1SKeys, Num1Sets };

/** CHAT KEYBOARD **/
static const GVSpecialKey Eng1SKeys[] = {
        { "\001", 0, GVKEY_SINGLESET, 1 },				// \001 (1)	= Shift Lower to Upper
        { "\001", 0, GVKEY_INVERT|GVKEY_LOCKSET, 2 },	// \002 (2)	= Shift Upper to Upper Lock
        { "\002", 0, GVKEY_INVERT|GVKEY_LOCKSET, 0 },	// \003 (3)	= Shift Upper Lock to Lower
        { "123", 0, GVKEY_LOCKSET, 3 },					// \004 (4)	= Change to Numbers
        { "\010", "\b", 0, 0 },							// \005 (5)	= Backspace
        { "\015", "\r", 0, 0 },							// \006 (6)	= Enter 1
        { "\015", "\r", 0, 0 },							// \007 (7)	= Enter 2 (Short keycap)
        { "Sym", 0, GVKEY_LOCKSET, 4 },					// \010 (8)	= Change to Symbols
        { "aA", 0, GVKEY_LOCKSET, 0 },					// \011 (9)	= Change to Lower Alpha
};
static const char Eng1Set1Row0[] = "QWERTYUIOP";
static const char Eng1Set1Row1[] = "ASDFGHJKL";
static const char Eng1Set0Row3[] = "\004   \006\006";
static const char *Eng1Set0[] = { "qwertyuiop",	"asdfghjkl",	"\001zxcvbnm\005",	Eng1Set0Row3,		0 };
static const char *Eng1Set1[] = { Eng1Set1Row0,	Eng1Set1Row1,	"\002ZXCVBNM\005",	Eng1Set0Row3,		0 };
static const char *Eng1Set2[] = { Eng1Set1Row0,	Eng1Set1Row1,	"\003ZXCVBNM\005",	Eng1Set0Row3,		0 };
static const char *Eng1Set3[] = { "789",			"456",		"123",			"\01100",	0 };
static const GVKeySet Eng1Sets[] = { Eng1Set0, Eng1Set1, Eng1Set2, Eng1Set3, 0, 0 };
const GVKeyTable VirtualKeyboard_English2 = { Eng1SKeys, Eng1Sets };

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