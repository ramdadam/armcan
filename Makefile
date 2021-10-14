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
ARCH     = /opt/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-
SRCFLAGS = -ggdb -O1
CFLAGS   = 
CXXFLAGS = -fno-rtti
ASFLAGS  = 
LDFLAGS  = --specs=nosys.specs

SRC	 = Src/VirtualKeyBoard/vkeyboard.cpp
SRC	 += Src/Driver/can_driver.cpp
SRC	 += Src/Driver/sd_driver.cpp
SRC	 += Src/common/ImagePushButton.cpp
SRC	 += Src/common/notification_helper.cpp
SRC	 += Src/View/main_view.cpp
SRC	 += Src/View/can_settings_view.cpp
SRC	 += Src/View/can_status_view.cpp
SRC	 += Src/View/add_can_message.cpp
SRC	 += Src/View/edit_can_message.cpp
SRC	 += Src/View/can_view.cpp
SRC	 += Src/View/tx_can_view.cpp
SRC	 += Src/View/rx_can_view.cpp
SRC	 += Src/View/sd_settings_view.cpp
SRC	 += Src/common/can_gui_package.cpp
SRC      += Src/main.cpp \
./res/startup_stm32f746xx.s \
./res/Src/bsp_driver_sd.c \
./res/Src/sd_diskio.c \
./res/Src/fatfs.c \
./res/Src/freertos.c \
./Src/stm32f7xx_it.c \
./res/Src/stm32f7xx_hal_msp.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_can.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_sdmmc.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_sd.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c \
./res/Src/system_stm32f7xx.c \
./res/Middlewares/Third_Party/FatFs/src/diskio.c \
./res/Middlewares/Third_Party/FatFs/src/ff.c \
./res/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c \
./res/Middlewares/Third_Party/FatFs/src/option/syscall.c \
./res/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c

OBJS     =
DEFS     = GFX_OS_HEAP_SIZE=40960 STM32F746xx GFX_OS_NO_INIT CAN_INTERRUPT_PRIORITY=10 ENTER_ISR LEAVE_ISR CAN_HARDWARE_ENABLED MAX_SEMAPHORE_COUNT=50
#DEFS     = 
LIBS     =
INCPATH  = ./Inc
INCPATH  += ./Inc/View
INCPATH  += Inc/images
INCPATH  += Inc/common
INCPATH  += Inc/events
INCPATH  += Inc/Driver
INCPATH  += ./res/Drivers/CMSIS/Device/ST/STM32F7xx/Include
INCPATH  += ./res/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
INCPATH  += ./res/Drivers/CMSIS/Include
INCPATH  += ./Src/VirtualKeyBoard
INCPATH  += ./res/Middlewares/Third_Party/FatFs/src
LIBPATH  =
LDSCRIPT = 

##############################################################################################
# These should be at the end
#

include $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk
include $(GFXLIB)/tools/gmake_scripts/os_$(OPT_OS).mk
include $(GFXLIB)/tools/gmake_scripts/compiler_gcc.mk
# *** EOF ***
# -I./Inc -IInc/images -IInc/common -ISrc/VirtualKeyBoard -ISrc/Common -I./res/Drivers/CMSIS/Device/ST/STM32F7xx/Include -I./res/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I./res/Drivers/CMSIS/Include -I./ugfx -I./ugfx/3rdparty/tinygl-0.4-ugfx/include -I./ugfx/boards/base/STM32F746-Discovery -I./res/Drivers/STM32F7xx_HAL_Driver/Inc -I./ugfx/drivers/gdisp/STM32LTDC -I./res/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I./res/Middlewares/Third_Party/FreeRTOS/Source/include
# -I./Inc -IInc/images -IInc/common -ISrc/VirtualKeyBoard -ISrc/Common -I./res/Drivers/CMSIS/Device/ST/STM32F7xx/Include -I./res/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I./res/Drivers/CMSIS/Include -I./ugfx -I./ugfx/3rdparty/tinygl-0.4-ugfx/include -I./ugfx/boards/base/STM32F746-Discovery -I./res/Drivers/STM32F7xx_HAL_Driver/Inc -I./ugfx/drivers/gdisp/STM32LTDC -I./res/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I./res/Middlewares/Third_Party/FreeRTOS/Source/include
# -DGFX_OS_HEAP_SIZE=40960 -DSTM32F746xx -DGFX_OS_NO_INIT -DCAN_INTERRUPT_PRIORITY=10 -DENTER_ISR -DLEAVE_ISR -DCAN_HARDWARE_ENABLED -DMAX_SEMAPHORE_COUNT=50 -DGFX_USE_OS_FREERTOS=TRUE -DCORTEX_USE_FPU=TRUE -DUSE_FPU=hard -DTHUMB_PRESENT -DTHUMB_NO_INTERWORKING -mthumb -DTHUMB fakethumbfile.c