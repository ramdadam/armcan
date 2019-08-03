
include_directories(Src/common)
include_directories(Src/VirtualKeyBoard)
include_directories(Src/Common)

file(GLOB COMMON_SRCS
        "${PROJECT_SOURCE_DIR}/Src/common/*.cpp")
file(GLOB DRIVER_SRCS
        "${PROJECT_SOURCE_DIR}/Src/Driver/*.cpp")
file(GLOB VIEW_SRCS
        "${PROJECT_SOURCE_DIR}/Src/View/*.cpp")
file(GLOB VIRTUALKEYBOARD_SRCS
        "${PROJECT_SOURCE_DIR}/Src/VirtualKeyBoard/*.cpp")

file(GLOB SYS_SRCS
        "${PROJECT_SOURCE_DIR}/Src/main.cpp"
        "${PROJECT_SOURCE_DIR}/Src/stm32f7xx_hal_msp.c"
        "${PROJECT_SOURCE_DIR}/Src/stm32f7xx_it.c"
        "${PROJECT_SOURCE_DIR}/Src/system_stm32f7xx.c"
        )

