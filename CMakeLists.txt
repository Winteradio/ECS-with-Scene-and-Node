set(PROJECT_NAME ECSProject)

if (TARGET ${PROJECT_NAME})
    message(STATUS "# ${PROJECT_NAME} target is already defined. Skipping build configuration.")
    
    return()
endif()

cmake_minimum_required(VERSION 3.20)
project(${PROJECT_NAME} LANGUAGES CXX)

# ------ Create the library ------ #
include(${CMAKE_SOURCE_DIR}/external/CMake/Library.cmake)

set(LIBRARY_TYPE "SHARED" CACHE STRING "Build a shared library instead of a static one")

set(MAIN_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(HEADER_DIR ${MAIN_DIR}/include)
set(SOURCE_DIR ${MAIN_DIR}/src)
set(EXTERNAL_DIR ${MAIN_DIR}/external)

set(INCLUDE_DIR_LIST
    ${MAIN_DIR}/include
)

set(HEADER_LIST
    ${HEADER_DIR}/Utils/ECSUtils.h
)

set(SOURCE_LIST
    ${SOURCE_DIR}/Utils/ECSUtils.cpp
)

create_library(
    LIBRARY_TYPE ${LIBRARY_TYPE}
    TARGET_NAME ${PROJECT_NAME}
    PUBLIC_HEADERS ${HEADER_LIST}
    PRIVATE_SOURCES ${SOURCE_LIST}
)

install_library(
    TARGET_NAME ${PROJECT_NAME}
    INCLUDE_DIRS ${INCLUDE_DIR_LIST}
)

# ------ Set the project options ------ #
include(${CMAKE_SOURCE_DIR}/external/CMake/Project.cmake)

project_options()

target_include_directories(${PROJECT_NAME} PUBLIC ${EXTERNAL_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC MemoryProject ReflectionProject LogProject Rpcrt4.lib)

# ------ Add the sub modules ------ #
message(STATUS "- Add the Sub Project")

set(MEMORY_DEMO_FILE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/Memory Memory)

set(REFLECTION_DEMO_FILE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/Reflection Reflection)

set(CONTAINER_DEMO_FILE ON)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/Container Container)

set(LOG_DEMO_FILE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/Log Log)

# ------ Create the Demofile ------ #
option(ECS_DEMO_FILE "Build the demo file" ON)

if (ECS_DEMO_FILE)
    message(STATUS "# Create the demofile project")

    add_subdirectory(${MAIN_DIR}/demofile)
endif()