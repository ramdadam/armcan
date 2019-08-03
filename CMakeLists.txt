cmake_minimum_required(VERSION 3.17)
set(ARMCAN_HOME ${CMAKE_CURRENT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH .build)

if(NOT DEFINED ARMCAN_HOME)
    MESSAGE(FATAL_ERROR "Please set the (absolute) pfyl home path")
ENDIF()

SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a" ".dylib")
set(STM32_FAMILY F7)
set(STM32_CHIP STM32F746NG)
set(STM_BOARD STM32746G-Discovery)
set(STM32Cube_DIR /Users/ramadan/STM32Cube/Repository/STM32Cube_FW_F7_V1.16.0)
set(STM32BSP_INCLUDE_DIR /Users/ramadan/STM32Cube/Repository/STM32Cube_FW_F7_V1.16.0/Drivers/BSP/STM32746G-Discovery)
set(STM32BSP_SOURCES /Users/ramadan/STM32Cube/Repository/STM32Cube_FW_F7_V1.16.0/Drivers/BSP/STM32746G-Discovery)
set(FreeRTOS_INCLUDE_DIRS /home/mohamad/STM32Cube/Repository/STM32Cube_FW_F7_V1.15.0/Middlewares/Third_Party/FreeRTOS)

set(TOOLCHAIN_PREFIX /opt/gcc-arm-none-eabi-9-2019-q4-major)
set(TARGET_TRIPLET arm-none-eabi)
set(uGFX_DIR ${ARMCAN_HOME}/ugfx)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/gcc_stm32.cmake)

include(${ARMCAN_HOME}/src/CMakeLists.txt)
project(armcan)

ENABLE_LANGUAGE(ASM)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

SET(FREERTOS_HEAP_IMPL 4)
FIND_PACKAGE(CMSIS REQUIRED)
FIND_PACKAGE(STM32HAL COMPONENTS gpio uart tim sdram i2c dma can REQUIRED)
FIND_PACKAGE(FreeRTOS REQUIRED)
FIND_PACKAGE(uGFX COMPONENTS gfx gfile gfile_fs_rom gdisp_fonts gdisp_image gdisp_pixmap gboard gos gos_freertos gdisp gdisp_fonts driver_STM32LTDC
        driver_FT5336 ginput ginput_mouse gqueue gwin gevent gwin_button gwin_checkbox gwin_container gwin_frame
        gwin_keyboard gwin_keyboard_layout gwin_label gwin_list gwin_radio gwin_slider gwin_tabset gwin_textedit
        gwin_widget gtimer REQUIRED)

include(${PROJECT_SOURCE_DIR}/Src/CMakeLists.txt)
include_directories("${PROJECT_SOURCE_DIR}/Inc")
include_directories("${PROJECT_SOURCE_DIR}/Inc/common")
include_directories("${PROJECT_SOURCE_DIR}/Inc/Driver")
include_directories("${PROJECT_SOURCE_DIR}/Inc/View")
include_directories("${PROJECT_SOURCE_DIR}/Inc/events")
include_directories("${PROJECT_SOURCE_DIR}/Inc/images")
include_directories("${PROJECT_SOURCE_DIR}/ugfx/boards/base/STM32F746-Discovery")

INCLUDE_DIRECTORIES(
        ${CMSIS_INCLUDE_DIRS}
        ${STM32HAL_INCLUDE_DIR}
        ${FreeRTOS_INCLUDE_DIRS}
        ${STM32BSP_INCLUDE_DIR}
        ${uGFX_INCLUDE_DIRS}
)

set(PROJECT_SOURCES
        ${COMMON_SRCS}
        ${DRIVER_SRCS}
        ${VIEW_SRCS}
        ${VIRTUALKEYBOARD_SRCS}
        ${SYS_SRCS}
        Src/main.cpp
        )

add_compile_options("-Wa,-mimplicit-it=thumb")

SET(STM32_LINKER_SCRIPT ${CMSIS_LINKER_SCRIPT})
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME})
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${PROJECT_SOURCES} ${SOURCE_HEAP_SOURCE_FILES} ${STM32BSP_SOURCES} ${FreeRTOS_SOURCES} ${uGFX_SOURCES}
         ${CMSIS_SOURCES} ${STM32HAL_SOURCES})


target_link_options(${CMAKE_PROJECT_NAME} PUBLIC -specs=nano.specs -specs=nosys.specs)
STM32_SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME})
STM32_ADD_HEX_BIN_TARGETS(${CMAKE_PROJECT_NAME})

