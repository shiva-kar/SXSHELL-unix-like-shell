cmake_minimum_required(VERSION 3.13)

project(unix-like-shell)

file(GLOB_RECURSE SOURCE_FILES src/*.c src/*.h)

set(CMAKE_C_STANDARD 23) # Enable the C23 standard

add_executable(shell ${SOURCE_FILES})

target_include_directories(shell PRIVATE include)
target_link_libraries(shell PRIVATE readline)
