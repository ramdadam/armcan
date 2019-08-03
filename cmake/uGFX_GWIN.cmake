SET(uGFX_GWIN_MODULES gwin_button gwin_checkbox gwin_container gwin_frame gwin_keyboard gwin_keyboard_layout gwin_label
        gwin_list gwin_radio gwin_slider gwin_tabset gwin_textedit gwin_widget)

SET(uGFX_gwin_SEARCH_PATH ${uGFX_DIR}/src/gwin)
SET(uGFX_gwin_HEADERS gwin.h gwin_class.h gwin_options.h gwin_rules.h)
SET(uGFX_gwin_SOURCES gwin_wm.c gwin.c)

FOREACH(module ${uGFX_GWIN_MODULES})
    SET(uGFX_${module}_SEARCH_PATH ${uGFX_DIR}/src/gwin)
    SET(uGFX_${module}_SOURCES ${module}.c)
    SET(uGFX_${module}_HEADERS ${module}.h)
ENDFOREACH()

