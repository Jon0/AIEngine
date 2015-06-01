cmake_minimum_required (VERSION 2.6)

project (AIEngine)

set(LIBRARY_NAME "AIEngine")
set(CPP_PATH "Source")

set(TEST_NAME "Test")
set(TEST_PATH "Test")

file(GLOB SRC_FILES
	"${CPP_PATH}/*.cpp"
)

file(GLOB INCLUDE_FILES
    "${CPP_PATH}/*.h"
)

file(GLOB MAIN_FILES
	"${TEST_PATH}/*.cpp"
)

# use c++14 standard library
add_definitions(-std=c++14)

# the main library
add_library(
    ${LIBRARY_NAME}
    SHARED
    ${SRC_FILES}
    ${INCLUDE_FILES}
)
target_link_libraries (${LIBRARY_NAME})

# a sample executable
include_directories(${CPP_PATH})
add_executable (${TEST_NAME} ${MAIN_FILES})
target_link_libraries (${TEST_NAME} ${LIBRARY_NAME})