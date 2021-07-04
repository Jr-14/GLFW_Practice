cmake_minimum_required(VERSION 3.20.0)

add_executable(openGLApp
  main.cpp
  glad.c
  shader.cpp
)

target_include_directories(openGLApp PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(openGLApp glfw)

find_package(OpenGL REQUIRED)

target_link_libraries(openGLApp OpenGL::GL)

# Compile Options on Unix machines
if (UNIX AND NOT APPLE)
  add_compile_options(-Wall -lglfw3 -lopengl32 -lgdi32)
endif ()

