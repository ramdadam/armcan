SET(uGFX_GBOARD_SUPPORT_MODULES)

MESSAGE(${uGFX_DIR}/boards/base/${STM_BOARD})
SET(uGFX_gboard_SEARCH_PATH ${uGFX_DIR}/boards/base/STM32F746-Discovery)
SET(uGFX_gboard_HEADERS stm32f7_i2c.h stm32f746g_discovery_sdram.h)
SET(uGFX_gboard_SOURCES stm32f7_i2c.c stm32f746g_discovery_sdram.c)
