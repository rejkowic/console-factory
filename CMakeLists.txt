cmake_minimum_required(VERSION 3.9)

project(console-factory)

add_subdirectory(test EXCLUDE_FROM_ALL)
add_subdirectory(src/console)

set(CMAKE_CXX_STANDARD 17)
add_library(${PROJECT_NAME} src/ConsoleFactory.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC interface)
target_link_libraries(${PROJECT_NAME} console)
