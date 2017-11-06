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
//#define GFX_USE_OS_FREERTOS                          FALSE
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
    #define GFX_OS_HEAP_SIZE                         40960
//    #define GFX_OS_NO_INIT                           FALSE
    #define GFX_OS_INIT_NO_WARNING                   TRUE
//    #define GFX_OS_PRE_INIT_FUNCTION                 Raw32OSInit
//    #define GFX_OS_EXTRA_INIT_FUNCTION               myOSInitRoutine
//    #define GFX_OS_EXTRA_DEINIT_FUNCTION             myOSDeInitRoutine
//    #define GFX_EMULATE_MALLOC                       FALSE

/* GFX sub-systems to turn on */
#define GFX_USE_GDISP                   TRUE

/* Features for the GDISP sub-system. */
#define GDISP_NEED_VALIDATION           FALSE
#define GDISP_NEED_CLIP                 FALSE
#define GDISP_NEED_TEXT                 FALSE
#define GDISP_NEED_CIRCLE               TRUE
#define GDISP_NEED_ELLIPSE              FALSE
#define GDISP_NEED_ARC                  FALSE
#define GDISP_NEED_SCROLL               FALSE
#define GDISP_NEED_PIXELREAD            FALSE
#define GDISP_NEED_CONTROL              FALSE
#define GDISP_NEED_MULTITHREAD          FALSE
#define GDISP_NEED_ASYNC                FALSE
#define GDISP_NEED_MSGAPI               FALSE

/* Builtin Fonts */
#define GDISP_INCLUDE_FONT_SMALL        FALSE
#define GDISP_INCLUDE_FONT_LARGER       FALSE
#define GDISP_INCLUDE_FONT_UI1          FALSE
#define GDISP_INCLUDE_FONT_UI2          FALSE
#define GDISP_INCLUDE_FONT_LARGENUMBERS FALSE

#endif /* _GFXCONF_H */
