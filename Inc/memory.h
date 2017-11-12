/** ***********************************************************************
 * @file		memory.h
 * @brief		special memory area declarations
 * @author		Dr. Klaus Schaefer
 **************************************************************************/

 #ifndef MEMORY_H_
 #define MEMORY_H_
 
 #include "stdint.h"
 
 extern uint32_t __common_data_start__[];
 extern uint32_t __common_data_end__[];
 #define COMMON_SIZE 1024
 #define COMMON_BLOCK __common_data_start__
 #define COMMON __attribute__ ((section ("common_data")))
 
 #define CONSTEXPR_ROM constexpr __attribute__ ((section (".rodata")))
 
 #ifndef ROM
 #define ROM const __attribute__ ((section (".rodata")))
 #endif
 
 #endif /* MEMORY_H_ */
 