add_executable(assignment-0002 src/main.cpp)

target_link_libraries(assignment-0002 glfw GLEW GL common)
target_include_directories(assignment-0002 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/include)
