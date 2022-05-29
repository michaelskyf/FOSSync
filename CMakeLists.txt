cmake_minimum_required(VERSION 3.23)

project(fossync)
set(fossync_verion 0.01-alpha)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

# Compiler stuff
set(CMAKE_C_STANDARD 11)

# Flags
set(CMAKE_C_FLAGS "-Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wundef")

# Definitions
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DFOSSYNC_VERSION=\\\"${fossync_verion}\\\"")

# Debug/Release flags
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

set(CMAKE_C_FLAGS_RELEASE "-Werror")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(lib)
add_subdirectory(src)
