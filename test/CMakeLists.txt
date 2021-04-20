cmake_minimum_required(VERSION 3.5)

project(chun_o2_test)

add_definitions("-DNATIVE")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}-std=c++17 -O0 -g -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -Wall")

include_directories(
    ${PROJECT_SOURCE_DIR}/../include
    /usr/include
)

add_executable(test_collection test_collect.cpp
    ../src/collect.cpp
    ../src/display.cpp
)

target_link_libraries(test_collection
    gtest
)