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
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

set(INCLUDE_DIR_LIST
    ${MAIN_DIR}/include
)

set(HEADER_LIST
    ${HEADER_DIR}/Object/Object.h
    ${HEADER_DIR}/Object/Data.h
    ${HEADER_DIR}/Object/Entity.h
    ${HEADER_DIR}/Object/Scene.h
    ${HEADER_DIR}/Object/System.h

    ${HEADER_DIR}/Container/BaseContainer.h
    ${HEADER_DIR}/Container/ComponentContainer.h
    ${HEADER_DIR}/Container/EntityContainer.h
    ${HEADER_DIR}/Container/NodeContainer.h

    ${HEADER_DIR}/Graph/Graph.h
    
    ${HEADER_DIR}/Registry/ContainerRegistry.h
    ${HEADER_DIR}/Registry/ComponentRegistry.h
    ${HEADER_DIR}/Registry/NodeRegistry.h
    ${HEADER_DIR}/Registry/SystemRegistry.h

    ${HEADER_DIR}/UUID/UUID.h

    ${HEADER_DIR}/TimeStep.h
    ${HEADER_DIR}/Utils.h
    ${HEADER_DIR}/ECS.h
)

set(SOURCE_LIST
    ${SOURCE_DIR}/Object/Object.cpp
    ${SOURCE_DIR}/Object/Entity.cpp
    ${SOURCE_DIR}/Object/Scene.cpp
    ${SOURCE_DIR}/Object/System.cpp

    ${SOURCE_DIR}/Container/BaseContainer.cpp
    ${SOURCE_DIR}/Container/EntityContainer.cpp

    ${SOURCE_DIR}/Registry/SystemRegistry.cpp

    ${SOURCE_DIR}/UUID/UUID.cpp
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
target_link_libraries(${PROJECT_NAME} PUBLIC MemoryProject ReflectionProject LogProject)

# ------ Add the sub modules ------ #
message(STATUS "- Add the Sub Project")

set(MEMORY_DEMO_FILE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/Memory Memory)

set(REFLECTION_DEMO_FILE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/Reflection Reflection)

set(CONTAINER_DEMO_FILE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/Container Container)

set(LOG_DEMO_FILE OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/Log Log)

# ------ Create the Demofile ------ #
option(ECS_DEMO_FILE "Build the demo file" ON)

if (ECS_DEMO_FILE)
    message(STATUS "# Create the demofile project")

    add_subdirectory(${MAIN_DIR}/demofile)
endif()