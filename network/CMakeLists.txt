
aux_source_directory(./ NET_SRCS)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs)
add_library(net ${NET_SRCS})
target_link_libraries(net core)
