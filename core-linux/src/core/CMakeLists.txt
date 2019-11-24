file(GLOB SOURCES **/*.cpp)

include_directories(../../lib)
add_library(core ${SOURCES})
