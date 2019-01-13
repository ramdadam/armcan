
#ifndef ARMCAN_VKEYBOARD_LAYOUT_H
#define ARMCAN_VKEYBOARD_LAYOUT_H

#include "src/gwin/gwin_keyboard_layout.h"
/** HEX KEYBOARD **/
static const GVSpecialKey Hex1SKeys[] = {{ "\010", "\b", 0, 0 } };

static const char Hex1Set1Row0[] = "123ABC";
static const char Hex1Set1Row1[] = "456DEF";
static const char Hex1Set0Row3[] = "7890\001";

static const char *Hex1Set0[] = { Hex1Set1Row0, Hex1Set1Row1, Hex1Set0Row3, nullptr, nullptr};

static const char *Hex1Set1[] = {nullptr, nullptr, nullptr, nullptr, nullptr};
static const char *Hex1Set2[] = {nullptr, nullptr, nullptr, nullptr, nullptr};
static const char *Hex1Set3[] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
static const char *Hex1Set4[] = {nullptr, nullptr, nullptr, nullptr, nullptr};
static const GVKeySet Hex1Sets[] = { Hex1Set0, Hex1Set1, Hex1Set2, Hex1Set3,
                                     Hex1Set4, nullptr};

static const GVKeyTable VirtualKeyboardHexInput = { Hex1SKeys, Hex1Sets };

/** NUMERIC KEYBOARD **/
static const GVSpecialKey Num1SKeys[] = {{ "\010", "\b", 0, 0 } };

static const char Num1Set1Row0[] = "1234";
static const char Num1Set1Row1[] = "5678";
static const char Num1Set0Row3[] = "90\001";

static const char *Num1Set0[] = { Num1Set1Row0, Num1Set1Row1, Num1Set0Row3, nullptr, nullptr};

static const char *Num1Set1[] = {nullptr, nullptr, nullptr, nullptr, nullptr};
static const char *Num1Set2[] = {nullptr, nullptr, nullptr, nullptr, nullptr};
static const char *Num1Set3[] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
static const char *Num1Set4[] = {nullptr, nullptr, nullptr, nullptr, nullptr};
static const GVKeySet Num1Sets[] = { Num1Set0, Num1Set1, Num1Set2, Num1Set3,
                                     Num1Set4, nullptr};

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
static const char Eng1Set1Row0[] = "QWERTYUIOP\005";
static const char Eng1Set1Row1[] = "ASDFGHJKL";
static const char Eng1Set0Row3[] = "\004         ";
static const char *Eng1Set0[] = { "qwertyuiop\005",	"asdfghjkl",	"\001zxcvbnm",	Eng1Set0Row3, nullptr};
static const char *Eng1Set1[] = { Eng1Set1Row0,	Eng1Set1Row1,	"\002ZXCVBNM",	Eng1Set0Row3, nullptr};
static const char *Eng1Set2[] = { Eng1Set1Row0,	Eng1Set1Row1,	"\003ZXCVBNM",	Eng1Set0Row3, nullptr};
static const char *Eng1Set3[] = { "789",			"456",		"123",			"\01100", nullptr};
static const GVKeySet Eng1Sets[] = { Eng1Set0, Eng1Set1, Eng1Set2, Eng1Set3, nullptr, nullptr};
const GVKeyTable VirtualKeyboard_English2 = { Eng1SKeys, Eng1Sets };
#endif