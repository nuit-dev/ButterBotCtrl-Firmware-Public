# CMakeLists for ESP-IDF

file(GLOB_RECURSE SOURCES "src/**.cpp" "src/**.c")
set(COMPONENT_SRCS ${SOURCES})

set(COMPONENT_ADD_INCLUDEDIRS src)

set(COMPONENT_REQUIRES CMF)

register_component()

# Custom (NUIT): phrase lookup runs rarely - optimize it for size, the robot's app partition is nearly full
set_source_files_properties(src/Phrases.cpp PROPERTIES COMPILE_OPTIONS "-Os")
