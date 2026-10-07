add_executable(assignment-0000 src/main.cpp)

target_link_libraries(assignment-0000 glfw GLEW GL common)
target_include_directories(assignment-0000 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/include)
