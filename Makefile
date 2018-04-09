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
ARCH     = /opt/gcc-arm-none-eabi-7-2017-q4-major/bin/arm-none-eabi-#/opt/gcc-arm-none-eabi-7-2017-q4-major-linux/bin/arm-none-eabi-
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
SRC  += \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c_ex.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_tim_ex.c \
./res/Middlewares/Third_Party/FreeRTOS/Source/tasks.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma.c \
./res/Middlewares/Third_Party/FreeRTOS/Source/queue.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_dma_ex.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_can.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_pwr_ex.c \
./res/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c \
./res/Src/stm32f7xx_it.c \
./Src/main.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_i2c.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_rcc_ex.c \
./res/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_gpio.c \
./res/Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
./res/Src/stm32f7xx_hal_msp.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_flash_ex.c \
./Src/freertos.c \
./res/Src/freertos.c \
./res/startup_stm32f746xx.s \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c \
./res/Middlewares/Third_Party/FreeRTOS/Source/list.c \
./res/Middlewares/Third_Party/FreeRTOS/Source/event_groups.c \
./res/Middlewares/Third_Party/FreeRTOS/Source/timers.c \
./res/Middlewares/Third_Party/FreeRTOS/Source/croutine.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_cortex.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/pppol2tp.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv6/ethip6.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/pppoe.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_addr.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/chap_ms.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv6/ip6.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/lcp.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/vj.c \
./res/Middlewares/Third_Party/LwIP/src/netif/lowpan6.c \
./Src/ethernetif.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv4/dhcp.c \
./res/Middlewares/Third_Party/LwIP/src/core/sys.c \
./res/Middlewares/Third_Party/LwIP/src/core/udp.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ethernet.c \
./res/Middlewares/Third_Party/LwIP/src/core/tcp_out.c \
./res/Middlewares/Third_Party/LwIP/src/netif/slipif.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv6/ip6_frag.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/pppos.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/demand.c \
./res/Middlewares/Third_Party/LwIP/system/OS/sys_arch.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/upap.c \
./res/Middlewares/Third_Party/LwIP/src/core/netif.c \
./res/Middlewares/Third_Party/LwIP/src/api/netdb.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/ipv6cp.c \
./res/Middlewares/Third_Party/LwIP/src/api/netbuf.c \
./res/Middlewares/Third_Party/LwIP/src/core/tcp_in.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_frag.c \
./res/Middlewares/Third_Party/LwIP/src/api/tcpip.c \
./Src/lwip.c \
./res/Middlewares/Third_Party/LwIP/src/core/raw.c \
./res/Middlewares/Third_Party/LwIP/src/api/err.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/chap-new.c \
./res/Middlewares/Third_Party/LwIP/src/api/api_msg.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4_addr.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv4/autoip.c \
./res/Drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_eth.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/ecp.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv4/icmp.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/ppp.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/magic.c \
./res/Middlewares/Third_Party/LwIP/src/core/pbuf.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/auth.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv6/nd6.c \
./res/Middlewares/Third_Party/LwIP/src/core/tcp.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/eui64.c \
./res/Middlewares/Third_Party/LwIP/src/core/memp.c \
./Src/system_stm32f7xx.c \
./res/Middlewares/Third_Party/LwIP/src/core/timeouts.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/ccp.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/pppapi.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv4/etharp.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv6/inet6.c \
./res/Middlewares/Third_Party/LwIP/src/core/init.c \
./res/Middlewares/Third_Party/LwIP/src/api/sockets.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv6/dhcp6.c \
./res/Middlewares/Third_Party/LwIP/src/api/api_lib.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv4/igmp.c \
./res/Middlewares/Third_Party/LwIP/src/core/def.c \
./res/Middlewares/Third_Party/LwIP/src/core/stats.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/fsm.c \
./res/Middlewares/Third_Party/LwIP/src/core/dns.c \
./res/Middlewares/Third_Party/LwIP/src/core/mem.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/mppe.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/utils.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/chap-md5.c \
./res/Middlewares/Third_Party/LwIP/src/core/ip.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv6/icmp6.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/eap.c \
./res/Middlewares/Third_Party/LwIP/src/core/inet_chksum.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv6/mld6.c \
./res/Middlewares/Third_Party/LwIP/src/core/ipv4/ip4.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/ipcp.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/pppcrypt.c \
./res/Middlewares/Third_Party/LwIP/src/api/netifapi.c \
./res/Middlewares/Third_Party/LwIP/src/netif/ppp/multilink.c

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

INCPATH += ./res/Drivers/STM32F7xx_HAL_Driver/Inc 
INCPATH += ./res/Inc 
INCPATH += ./res/Drivers/STM32F7xx_HAL_Driver/Inc/Legacy 
INCPATH += ./res/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 
INCPATH += ./res/Drivers/CMSIS/Device/ST/STM32F7xx/Include 
INCPATH += ./res/Middlewares/Third_Party/FreeRTOS/Source/include 
INCPATH += ./res/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS 
INCPATH += ./res/Drivers/CMSIS/Include 
INCPATH += ./res/Middlewares/Third_Party/LwIP/src/include 
INCPATH += ./res/Middlewares/Third_Party/LwIP/system 
INCPATH += ./res/Middlewares/Third_Party/LwIP/src/include/netif/ppp 
INCPATH += ./res/Middlewares/Third_Party/LwIP/src/include/lwip 
INCPATH += ./res/Middlewares/Third_Party/LwIP/src/include/lwip/apps 
INCPATH += ./res/Middlewares/Third_Party/LwIP/src/include/lwip/priv 
INCPATH += ./res/Middlewares/Third_Party/LwIP/src/include/lwip/prot 
INCPATH += ./res/Middlewares/Third_Party/LwIP/src/include/netif 
INCPATH += ./res/Middlewares/Third_Party/LwIP/src/include/posix 
INCPATH += ./res/Middlewares/Third_Party/LwIP/src/include/posix/sys 
INCPATH += ./res/Middlewares/Third_Party/LwIP/system/arc

LIBPATH  =
LDSCRIPT = 

##############################################################################################
# These should be at the end
#

include $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk
include $(GFXLIB)/tools/gmake_scripts/os_$(OPT_OS).mk
include $(GFXLIB)/tools/gmake_scripts/compiler_gcc.mk
# *** EOF ***
