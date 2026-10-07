cmake_minimum_required(VERSION 3.10)
project(opengl_class CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 3.3 REQUIRED)

add_subdirectory(shared)
add_subdirectory(assignments/0000-template)
