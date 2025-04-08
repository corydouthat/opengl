cmake_minimum_required(VERSION 3.8)

project(opengl LANGUAGES C CXX ASM)
set(CMAKE_CXX_STANDARD 17)

if (NOT TARGET opengl)
	add_library(opengl INTERFACE)
endif()

add_definitions(-DGLEW_STATIC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(dependencies/libraries libraries)
add_subdirectory(dependencies/assimp assimp)
add_subdirectory(dependencies/glfw glfw)
add_subdirectory(dependencies/glew-cmake EXCLUDE_FROM_ALL)
add_subdirectory(dependencies/soil EXCLUDE_FROM_ALL)

# Add link libraries
target_link_libraries(opengl 
	INTERFACE libraries
	INTERFACE assimp
	INTERFACE glfw
	INTERFACE libglew_static
	INTERFACE soil
	#dependencies/static/glew-2.1.0/lib/Release/Win32/glew32 #TODO: check 32 vs 64-b
	#INTERFACE dependencies/static/SOIL/lib/SOIL #TODO: use .a file for Linux
)

# Add include directories
target_include_directories(opengl INTERFACE .)

# Example to include a local main function to test building the library
#add_executable(opengl-testbench main.cpp)
#target_link_libraries(opengl-testbench 
#	PRIVATE libraries
#	PRIVATE assimp
#	PRIVATE glfw
#	PRIVATE libglew_static
#	PRIVATE soil
#)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET opengl PROPERTY CXX_STANDARD 20)
endif()
