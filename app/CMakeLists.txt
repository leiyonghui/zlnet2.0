
aux_source_directory(./ APP_SRCS)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_library(zlapp ${APP_SRCS})
target_link_libraries(zlapp core net)



