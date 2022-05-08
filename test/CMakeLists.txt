
aux_source_directory(./ TEST_SRCS)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(test1 ${TEST_SRCS})
target_link_libraries(test1 core net)



