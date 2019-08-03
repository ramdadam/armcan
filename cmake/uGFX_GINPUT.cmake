SET(uGFX_GINPUT_MODULES ginput_mouse ginput_keyboard ginput_keyboard_microcode ginput_toggle
        ginput_dial driver_FT5336)

SET(uGFX_ginput_SEARCH_PATH ${uGFX_DIR}/src/ginput)
SET(uGFX_ginput_HEADERS ginput.h ginput_options.h ginput_rules.h)
SET(uGFX_ginput_SOURCES ginput.c)

FOREACH(module ${uGFX_GINPUT_MODULES})
    SET(uGFX_${module}_SEARCH_PATH ${uGFX_DIR}/src/ginput)
    SET(uGFX_${module}_SOURCES ${module}.c)
    SET(uGFX_${module}_HEADERS ${module}.h)
ENDFOREACH()


SET(uGFX_GINPUT_DRIVER_TYPE touch)
SET(uGFX_GINPUT_DRIVERS FT5336)

FOREACH(driver ${uGFX_GINPUT_DRIVERS})
    FOREACH(driver_type ${uGFX_GINPUT_DRIVER_TYPE})
        string(TOLOWER ${driver} driver_lowered)
        SET(uGFX_driver_${driver}_SEARCH_PATH ${uGFX_DIR}/drivers/ginput/${driver_type}/${driver})
        SET(uGFX_driver_${driver}_SOURCES gmouse_lld_${driver}.c)
        SET(uGFX_driver_${driver}_HEADERS ${driver_lowered}.h)
        LIST(APPEND uGFX_GINPUT_MODULES driver_${driver})
    ENDFOREACH()
ENDFOREACH()
