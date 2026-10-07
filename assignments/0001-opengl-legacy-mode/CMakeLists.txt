add_executable(assignment-0001 src/main.cpp)

target_link_libraries(assignment-0001 glfw GLEW GL common)
target_include_directories(assignment-0001 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../shared/include)
