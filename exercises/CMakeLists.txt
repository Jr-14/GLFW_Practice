# Learn how to build different releases
cmake_minimum_required(VERSION 3.20)

add_executable(OpenGLApp 
	glad.c
	HelloTriangle5.8-Exercise3.cpp
)

target_link_libraries(OpenGLApp glfw)

find_package(OpenGL REQUIRED)

target_link_libraries(OpenGLApp OpenGL::GL)

add_compile_options(-lglfw3 -lopengl32 -lgdi32)