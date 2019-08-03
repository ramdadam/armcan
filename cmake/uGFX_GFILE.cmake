SET(uGFX_GFILE_MODULES gfile_fs_rom)

SET(uGFX_gfile_SEARCH_PATH ${uGFX_DIR}/src/gfile)
SET(uGFX_gfile_HEADERS gfile.h gfile_options.h gfile_rules.h gfile_fs.h)
SET(uGFX_gfile_SOURCES gfile.c)

FOREACH(module ${uGFX_GFILE_MODULES})
    SET(uGFX_${module}_SEARCH_PATH ${uGFX_DIR}/src/gfile)
    SET(uGFX_${module}_SOURCES ${module}.c)
ENDFOREACH()