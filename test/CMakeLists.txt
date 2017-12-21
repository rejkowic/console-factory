cmake_minimum_required(VERSION 3.9)

project(console-factory-test)

add_subdirectory(app-factory)

set(CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} console-factory-test.cpp)
target_link_libraries(${PROJECT_NAME} console-factory app-factory)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ln -sf ${CMAKE_CURRENT_SOURCE_DIR}/in.txt ${CMAKE_CURRENT_BINARY_DIR}
)
