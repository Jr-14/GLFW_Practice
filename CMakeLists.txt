cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GLFW_practice VERSION 1.0)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.3.4)

add_executable(test 
  main.cpp
  glad.c
) 


target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(test glfw)

find_package(OpenGL REQUIRED)

target_link_libraries(test OpenGL::GL)
