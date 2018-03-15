# Possible Targets:	all clean Debug cleanDebug Release cleanRelease

##############################################################################################
# Settings
#

# General settings
	# See $(GFXLIB)/tools/gmake_scripts/readme.txt for the list of variables
	OPT_OS					= freertos
	OPT_THUMB				= yes
	OPT_LINK_OPTIMIZE		= no
	OPT_CPU					= stm32m7
	FREERTOS				= ./res/Middlewares/Third_Party/FreeRTOS
	FREERTOS_BOARD			= ARM_CM7/r0p1
	FREERTOS_LDSCRIPT		= ./res/STM32F746NGHx_FLASH.ld
# uGFX settings
	# See $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk for the list of variables
	GFXLIB					= ./ugfx
	GFXBOARD				= STM32F746-Discovery
	#GFXDEMO					= applications/combo
	#GFXDRIVERS				=
	GFXSINGLEMAKE			= no


#Special - Required for the drivers for this discovery board.
STMHAL		= ./res/Drivers/STM32F7xx_HAL_Driver

#Special - Required for Raw32
CMSIS			= ./res/Drivers/CMSIS

##############################################################################################
# Set these for your project
#
ARCH     = /opt/gcc-arm-none-eabi-7-2017-q4-major-linux/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-#/opt/gcc-arm-none-eabi-7-2017-q4-major-linux/bin/arm-none-eabi-
SRCFLAGS = -ggdb -O1
CFLAGS   = -fdata-sections -ffunction-sections -Wfatal-errors
CXXFLAGS = -fno-rtti -Wfatal-errors -lstdc++ 
ASFLAGS  = -fdata-sections -ffunction-sections
LDFLAGS  = -specs=nano.specs -specs=nosys.specs -Wl,--gc-sections 

SRC	 = Src/VirtualKeyBoard/vkeyboard.cpp
SRC	 += Src/View/main_view.cpp
SRC	 += Src/View/add_can_message.cpp
SRC	 += Src/View/can_view.cpp
SRC	 += Src/View/tx_can_view.cpp
SRC	 += Src/View/rx_can_view.cpp
SRC      += Src/main.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_can.c \
./res/Src/system_stm32f7xx.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c \
./res/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c \
./res/Src/stm32f7xx_it.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c \
./res/Src/stm32f7xx_hal_msp.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c \
./res/Src/freertos.c \
./res/startup_stm32f746xx.s \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c  
OBJS     =
DEFS     = GFX_OS_HEAP_SIZE=40960 STM32F746xx GFX_OS_NO_INIT
#DEFS     = 
LIBS     =
INCPATH  = ./Inc
INCPATH += Src/VirtualKeyBoard
INCPATH += Src/Common
INCPATH += ./res/Drivers/CMSIS/Device/ST/STM32F7xx/Include
INCPATH += ./res/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
INCPATH += ./res/Drivers/CMSIS/Include
LIBPATH  =
LDSCRIPT = 

##############################################################################################
# These should be at the end
#

include $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk
include $(GFXLIB)/tools/gmake_scripts/os_$(OPT_OS).mk
include $(GFXLIB)/tools/gmake_scripts/compiler_gcc.mk
# *** EOF ***
