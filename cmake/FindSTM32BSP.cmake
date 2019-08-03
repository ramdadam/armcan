IF (STM32_FAMILY STREQUAL "F4")
    IF (STM_BOARD STREQUAL "STM32F429I-Discovery")
        SET(BSP_COMPONENTS eeprom
                gyroscope
                io
                lcd
                sdram
                ts)
        SET(BSP_PREFIX stm32f429i_discovery_)
        SET(BSP_HEADERS stm32f429i_discovery.h)
        SET(BSP_SRC stm32f429i_discovery.c)
    ENDIF ()
    set(COMMON_COMPONENTS ampire480272
            ampire640480
            cs43l22
            exc7200
            ili9325
            ili9341
            l3gd20
            lis302dl
            lis3dsh
            lsm303dlhc
            mfxstm32l152
            n25q256a
            ov2640
            s5k5cag
            st7735
            stmpe1600
            stmpe811
            ts3510
            wm8994
            )
ENDIF ()

IF (STM32_FAMILY STREQUAL "F7")
    IF (STM_BOARD STREQUAL "STM32746G-Discovery")
        SET(BSP_COMPONENTS eeprom
                audio
                camera
                lcd
                sd
                qspi
                sdram
                ts)
        SET(BSP_PREFIX stm32746g_discovery_)
        SET(BSP_HEADERS stm32746g_discovery.h)
        SET(BSP_SRC stm32746g_discovery.c)
    ENDIF ()
    set(COMMON_COMPONENTS adv7533
            ampire480272
            ampire640480
            exc7200
            ft6x06
            ft5336
            mfxstm32l152
            mx25l512
            n25q128a
            n25q512a
            otm8009a
            ov9655
            rk043fn48h
            s5k5cag
            st7735
            st7789h2
            stmpe811
            ts3510
            wm8994
            )
ENDIF ()

IF (NOT STM32BSP_FIND_COMPONENTS)
    SET(STM32BSP_FIND_COMPONENTS ${BSP_COMPONENTS} ${COMMON_COMPONENTS})
    MESSAGE(STATUS "No STM32BSP components selected, using all: ${STM32BSP_FIND_COMPONENTS}")
ENDIF ()

FOREACH (cmp ${STM32BSP_FIND_COMPONENTS})
    LIST(FIND BSP_COMPONENTS ${cmp} STM32BSP_FOUND_INDEX)
    MESSAGE(STATUS FIND BSP_COMPONENTS ${cmp} STM32BSP_FOUND_INDEX)
    IF (${STM32BSP_FOUND_INDEX} LESS 0)
        LIST(FIND COMMON_COMPONENTS ${cmp} COMMON_FOUND_INDEX)
        IF (${COMMON_FOUND_INDEX} LESS 0)
            MESSAGE(FATAL_ERROR "Unknown STM32BSP component: ${cmp}. Available components: ${BSP_COMPONENTS} and ${COMMON_COMPONENTS}")
        ELSE ()
            LIST(APPEND BSP_COMMON_HEADER ${cmp}.h)
            LIST(APPEND BSP_SRC ${cmp}.c)
        ENDIF ()
    ELSE ()
        LIST(APPEND BSP_HEADERS ${BSP_PREFIX}${cmp}.h)
        LIST(APPEND BSP_SRC ${BSP_PREFIX}${cmp}.c)
    ENDIF ()
ENDFOREACH ()

FIND_PATH(STM32BSP_INCLUDE_DIR ${BSP_HEADERS}
        HINTS ${STM32Cube_DIR}/Drivers/BSP/${STM_BOARD}
        CMAKE_FIND_ROOT_PATH_BOTH
        )

FOREACH (cmp ${BSP_COMMON_HEADERS})
    FIND_PATH(STM32BSP_${cmp}_INCLUDE_DIR ${cmp}
            HINTS ${STM32Cube_DIR}/Drivers/BSP/Components
            PATH_SUFFIXES ${cmp}
            CMAKE_FIND_ROOT_PATH_BOTH
            )
    MESSAGE(STATUS "BSP include file is " STM32BSP_${cmp}_INCLUDE_DIR)
    LIST(APPEND BSP_HEADERS ${STM32BSP_${cmp}_INCLUDE_DIR})
ENDFOREACH ()

FOREACH (file ${BSP_SRC})
    STRING(REPLACE ".c" "" subfolder ${file})
    FIND_FILE(${file} ${file}
            HINTS ${STM32Cube_DIR}/Drivers/BSP/${STM_BOARD} ${STM32Cube_DIR}/Drivers/BSP/Components
            PATH_SUFFIXES ${subfolder}
            CMAKE_FIND_ROOT_PATH_BOTH
            )
    MESSAGE(STATUS "BSP file is " ${${file}})
    LIST(APPEND STM32BSP_SOURCES ${${file}})
ENDFOREACH ()

INCLUDE(FindPackageHandleStandardArgs)

INCLUDE_DIRECTORIES(${STM32Cube_DIR}/Drivers/BSP/${STM_BOARD}STM32746G-Discovery)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(STM32BSP DEFAULT_MSG STM32BSP_INCLUDE_DIR STM32BSP_SOURCES)
