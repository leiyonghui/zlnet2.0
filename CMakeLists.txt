cmake_minimum_required(VERSION 3.10)

project(lyhServer)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

message(status "build type: .....${CMAKE_BUILD_TYPE}")

add_definitions(-D_MONITOR)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	message(status "SYSTEM: ....liunx")
	add_definitions(-D__Linux -D__linux)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	message(status "SYSTEM: ....Windows")
	add_definitions(-D__Win32)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

#set(CMAKE_CXX_FLAGS_DEBUG -g -std=c++17)
#set(CMAKE_CXX_FLAGS_RELEASE -g -std=c++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -pthread")
include_directories(${PROJECT_SOURCE_DIR})
include_directories(./core/)

#message(status "project sdir: .....${PROJECT_SOURCE_DIR}")
#message(status "LIBRARY_OUTPUT_PATH: .....${LIBRARY_OUTPUT_PATH}")

add_subdirectory(core)
add_subdirectory(network)
add_subdirectory(engine)
add_subdirectory(app)
add_subdirectory(test)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs) 

#message(status "LIBRARY_OUTPUT_PATH: .....${LIBRARY_OUTPUT_PATH}")
