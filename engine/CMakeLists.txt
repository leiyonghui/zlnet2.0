
aux_source_directory(./ ENGING_SRCS)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs)

add_library(engine ${ENGING_SRCS})
target_link_libraries(engine core net)



