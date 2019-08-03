/**
 * This file has a different license to the rest of the GFX system.
 * You can copy, modify and distribute this file as you see fit.
 * You do not need to publish your source modifications to this file.
 * The only thing you are not permitted to do is to relicense it
 * under a different license.
 */

#ifndef _GFXCONF_H
#define _GFXCONF_H

/* The operating system to use. One of these must be defined - preferably in your Makefile */
//#define GFX_USE_OS_CHIBIOS                           FALSE
#define GFX_USE_OS_FREERTOS                          TRUE
#define GFX_OS_NO_INIT TRUE
//    #define GFX_FREERTOS_USE_TRACE                   FALSE
//#define GFX_USE_OS_WIN32                             FALSE
//#define GFX_USE_OS_LINUX                             FALSE
//#define GFX_USE_OS_OSX                               FALSE
//#define GFX_USE_OS_ECOS                              FALSE
//#define GFX_USE_OS_RAWRTOS                           FALSE
//#define GFX_USE_OS_ARDUINO                           FALSE
//#define GFX_USE_OS_KEIL                              FALSE
//#define GFX_USE_OS_CMSIS                             FALSE
//#define GFX_USE_OS_RAW32                             TRUE
//#define GFX_USE_OS_NIOS                              FALSE
//    #define INTERRUPTS_OFF()                         optional_code
//    #define INTERRUPTS_ON()                          optional_code

// Options that (should where relevant) apply to all operating systems
//    #define GFX_NO_INLINE                            FALSE
//    #define GFX_COMPILER                             GFX_COMPILER_UNKNOWN
//    #define GFX_CPU                                  GFX_CPU_CORTEX_M7_FPU
//    #define GFX_CPU_NO_ALIGNMENT_FAULTS              FALSE
//    #define GFX_CPU_ENDIAN                           GFX_CPU_ENDIAN_UNKNOWN

#define GDISP_INCLUDE_FONT_DEJAVUSANS32	TRUE
#define GWIN_NEED_TABLE                 TRUE
#define GWIN_NEED_TABLE_IMAGES          FALSE
#define GWIN_NEED_TABLE_TITLE			TRUE
#define GWIN_NEED_TABLE_HEADER			TRUE
#define GWIN_NEED_TABLE_EMBELLISH		TRUE
#define GFILE_MAX_GFILES 		        4
#define GDISP_NEED_STARTUP_LOGO 		0
#define GDISP_STARTUP_LOGO_TIMEOUT 		0
#define GFX_USE_GWIN					TRUE
#define GFX_OS_HEAP_SIZE				40960
#define GFX_OS_INIT_NO_WARNING			TRUE
#define GWIN_BUTTON_LAZY_RELEASE        TRUE
#define GFX_USE_GINPUT					TRUE
#define GFX_EMULATE_MALLOC              TRUE
/* GFX sub-systems to turn on */
#define GFX_USE_GDISP                   TRUE
#define GFX_USE_GEVENT					TRUE
#define GQUEUE_NEED_GSYNC				TRUE
/* Features for the GDISP sub-system. */
#define GDISP_NEED_VALIDATION           FALSE
#define GDISP_NEED_CLIP                 TRUE
#define GDISP_NEED_CIRCLE               TRUE
#define GDISP_NEED_ELLIPSE              FALSE
#define GDISP_NEED_ARC                  FALSE
#define GDISP_NEED_SCROLL               FALSE
#define GDISP_NEED_PIXELREAD            TRUE
#define GDISP_NEED_CONTROL              FALSE
#define GDISP_NEED_ASYNC                FALSE
#define GDISP_NEED_MSGAPI               FALSE
#define GWIN_NEED_WIDGET                TRUE
#define GWIN_NEED_FRAME                 TRUE
#define GWIN_NEED_KEYBOARD              TRUE
#define GWIN_NEED_CHECKBOX              TRUE
#define GWIN_NEED_SLIDER              	TRUE
#define GWIN_LABEL_ATTRIBUTE            TRUE
#define GWIN_NEED_TEXTEDIT				TRUE
#define GWIN_NEED_CONTAINERS            TRUE
#define GWIN_NEED_CONTAINER            TRUE
#define GDISP_NEED_TEXT					TRUE
#define GWIN_NEED_LABEL					TRUE
#define GWIN_NEED_BUTTON                TRUE
#define GWIN_NEED_RADIO                 TRUE
#define GWIN_NEED_TABSET			    TRUE
#define GWIN_NEED_LIST					TRUE
#define GWIN_NEED_IMAGE					TRUE
#define GDISP_NEED_IMAGE					TRUE
/* Builtin Fonts */
#define GDISP_INCLUDE_FONT_SMALL        FALSE
#define GDISP_INCLUDE_FONT_LARGER       FALSE
#define GDISP_INCLUDE_FONT_UI1          FALSE
#define GDISP_INCLUDE_FONT_UI2          FALSE
#define GDISP_INCLUDE_FONT_LARGENUMBERS FALSE
#define GDISP_INCLUDE_FONT_DEJAVUSANS12 TRUE
#define GDISP_INCLUDE_FONT_DEJAVUSANS16 TRUE
#define GDISP_INCLUDE_FONT_DEJAVUSANS20	TRUE
#define GDISP_INCLUDE_FONT_DEJAVUSANS24	TRUE
#define GINPUT_NEED_MOUSE				TRUE
#define GDISP_NEED_CONVEX_POLYGON		TRUE
#define GDISP_NEED_MULTITHREAD			TRUE
#define GWIN_NEED_WINDOWMANAGER			TRUE
#define GFILE_NEED_MEMFS			TRUE
#define GFX_USE_GTIMER					TRUE
#define GFX_USE_GQUEUE					TRUE
#define GFX_USE_GFILE                                TRUE

#define GFILE_NEED_ROMFS                             TRUE
#define GQUEUE_NEED_ASYNC				TRUE
    #define GDISP_NEED_IMAGE_BMP                     TRUE
        #define GDISP_NEED_IMAGE_BMP_24              TRUE
#endif /* _GFXCONF_H */
