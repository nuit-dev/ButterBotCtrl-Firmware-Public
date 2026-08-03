file(GLOB_RECURSE LIBS "../lib/*/src/**.cpp" "../lib/*/src/**.c")
set(LIBS_INCL "")

file(GLOB_RECURSE SRC "src/*/**.cpp" "src/*/**.c")

idf_component_register(SRCS "main.cpp" ${SRC} ${LIBS} INCLUDE_DIRS "src" ${LIBS_INCL})

spiffs_create_partition_image(storage ../spiffs_image FLASH_IN_PROJECT)
