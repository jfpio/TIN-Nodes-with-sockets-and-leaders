cmake_minimum_required(VERSION 3.16)
project(tin_long_live_the_king)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB_RECURSE project_SRC "src/*.cpp")
set(executable_NAME "long_live_the_king")

add_executable(${executable_NAME} ${project_SRC})

target_link_libraries(${executable_NAME} pthread)
target_compile_options(${executable_NAME} PRIVATE -Wall -Wextra -pedantic)