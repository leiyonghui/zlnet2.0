
aux_source_directory(./ ENGING_SRCS)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_library(engine ${ENGING_SRCS})
target_link_libraries(engine core net)



