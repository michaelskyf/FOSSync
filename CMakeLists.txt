cmake_minimum_required(VERSION 3.23)

project(fossync LANGUAGES C CXX)
set(fossync_verion 0.01-alpha)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

include(GoogleTest)
include(CTest)
find_package(GTest REQUIRED)
enable_testing()

# Compiler stuff
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# Flags
set(COMMON_FLAGS "-Wall")
set(COMMON_FLAGS "${COMMON_FLAGS} -Wextra")
set(COMMON_FLAGS "${COMMON_FLAGS} -Wundef")
set(COMMON_FLAGS "${COMMON_FLAGS} -pedantic")
set(COMMON_FLAGS "${COMMON_FLAGS} -Wno-gnu-statement-expression")
set(COMMON_FLAGS "${COMMON_FLAGS} -Wno-pointer-arith")
set(COMMON_FLAGS "${COMMON_FLAGS} -Werror")

set(COMMON_FLAGS "${COMMON_FLAGS} -pthread")

# Definitions
set(COMMON_FLAGS "${COMMON_FLAGS} -DFOSSYNC_VERSION=\\\"${fossync_verion}\\\"")

# Debug/Release flags
set(COMMON_FLAGS_DEBUG "-g")
set(COMMON_FLAGS_DEBUG "${COMMON_FLAGS_DEBUG} -O0")

set(COMMON_FLAGS_RELEASE "")
set(COMMON_FLAGS_RELEASE "${COMMON_FLAGS_RELEASE} -O3")

set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}")

set(CMAKE_C_FLAGS_DEBUG "${COMMON_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_FLAGS_DEBUG}")

set(CMAKE_C_FLAGS_RELEASE "${COMMON_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_FLAGS_RELEASE}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
