cmake_minimum_required(VERSION 3.16)
project(tin_long_live_the_king)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB_RECURSE project_SRC "src/*.cpp")
list(FILTER project_SRC EXCLUDE REGEX "src/main.cpp")
file(GLOB_RECURSE project_MAIN "src/main.cpp")
file(GLOB_RECURSE project_HEADER "include/*.h")

set(project_library_NAME "tin_lib")
set(executable_NAME "long_live_the_king")

add_library(${project_library_NAME} ${project_SRC} ${project_HEADER})

add_executable(${executable_NAME} ${project_MAIN})

target_link_libraries(${executable_NAME} ${project_library_NAME})
target_link_libraries(${executable_NAME} pthread)
target_compile_options(${executable_NAME} PRIVATE -Wall -Wextra -pedantic)

option(BUILD_TESTS "Build boost tests for the project" OFF)
if(BUILD_TESTS)
    add_subdirectory(test)
endif()