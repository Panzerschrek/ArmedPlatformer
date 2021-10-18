cmake_minimum_required(VERSION 3.10)

project(ArmedPlatformer)

# Setup language options.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

# Setup compiler options.
if(MSVC)
	add_compile_options(/utf-8 /W3 /WX)
	add_definitions(
		-DNOMINMAX
		-D_CRT_SECURE_NO_WARNINGS)
else()
	add_compile_options(-Werror -Wall -Wextra -Wconversion)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions(-DDEBUG)
endif()

if(WIN32)
	set(SDL2_INCLUDE_DIRS ${SDL2_DIR}/include)
	set(SDL2_LIBRARIES ${SDL2_DIR}/lib/x64/SDL2.lib)
else()
	find_package(SDL2 REQUIRED)
endif()

file(GLOB_RECURSE SOURCES "*.cpp" "*.hpp")
add_executable(ArmedPlatformer ${SOURCES})
target_include_directories(ArmedPlatformer PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(ArmedPlatformer PUBLIC ${SDL2_LIBRARIES})
