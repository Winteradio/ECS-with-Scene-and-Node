# ------ Set Options for Projects --------------- #
MESSAGE( STATUS "Set Options for Project")

## CMake minimun version required
CMAKE_MINIMUM_REQUIRED( VERSION 3.20 )

## Make Project Name
SET( PROJECT_NAME "ECSProject" )
## Project name
PROJECT( ${PROJECT_NAME} )

SET(CMAKE_BUILD_TYPE Debug)
if( MSVC )
	MESSAGE( STATUS "Compiler is ${CMAKE_CXX_COMPILER_ID}")
    # Use multi-process debugging
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /MP")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Zi /MP")

    # Use Windbg as the debugger
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DEBUG:FULL")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /DEBUG:FULL")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} /DEBUG /INCREMENTAL:NO")
endif()

## Set CXX Compiler Version
SET( CMAKE_CXX_STANDARD 17 )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Files ------------------------------ #
MESSAGE( STATUS "Set Source File")

SET( MAIN_DIR ${CMAKE_SOURCE_DIR} )
SET( INTERFACE_DIR ${MAIN_DIR}/Interface )
SET( MANAGER_DIR ${MAIN_DIR}/Manager )
SET( UTILS_DIR ${MAIN_DIR}/Utils )
SET( UUID_DIR ${MAIN_DIR}/UUID )
SET( DEMOFILE_DIR ${MAIN_DIR}/demofile )

SET( SOURCE_DIR )
SET( FOLDER_DIR )

## Source Files
LIST( APPEND SOURCE_DIR

    ${INTERFACE_DIR}/IComponent.h
    ${INTERFACE_DIR}/INode.h
    ${INTERFACE_DIR}/ISystem.h
    ${INTERFACE_DIR}/Entity.h ${INTERFACE_DIR}/Entity.cpp
    ${INTERFACE_DIR}/Scene.h ${INTERFACE_DIR}/Scene.cpp

    ${MANAGER_DIR}/ComponentManager.h ${MANAGER_DIR}/ComponentManager.cpp
    ${MANAGER_DIR}/EntityManager.h ${MANAGER_DIR}/EntityManager.cpp
    ${MANAGER_DIR}/NodeManager.h ${MANAGER_DIR}/NodeManager.cpp
    ${MANAGER_DIR}/SceneManager.h ${MANAGER_DIR}/SceneManager.cpp
    ${MANAGER_DIR}/SystemManager.h ${MANAGER_DIR}/SystemManager.cpp

    ${UTILS_DIR}/Utils.h

    ${UUID_DIR}/MyUUID.h ${UUID_DIR}/MyUUID.cpp

    )

## Header Files
LIST( APPEND FOLDER_DIR

    ${INTERFACE_DIR}
    ${MANAGER_DIR}
    ${UTILS_DIR}
    ${UUID_DIR}
)

MESSAGE( STATUS "Setting Done ")
# ----------------------------------------------- #



# ------ Set Options for Target Files ----------- #
MESSAGE( STATUS "Set Option" )

OPTION( BUILD_STATIC_LIBRARY "Set option for the library which is static or dynamic" OFF )
OPTION( INSTALL_DEMO_FILE "Install demofile for memorymanager" OFF )
## Set Option for install lib, header files' path
SET( PREFIX_DIR ${MAIN_DIR}/build/Prefix CACHE PATH "Prefix Path" )
SET( INCLUDE_DIR ${MAIN_DIR}/build/include CACHE PATH "Header files Path" )
SET( ARC_DIR ${MAIN_DIR}/build/arc CACHE PATH "Archieve files Path" )
SET( LIB_DIR ${MAIN_DIR}/build/lib CACHE PATH "Library files Path" )
SET( BIN_DIR ${MAIN_DIR}/build/bin CACHE PATH "Execute files Path" )
SET( CMAKE_BUILD_TYPE Debug CACHE STRING "Build Type" )

INCLUDE( Dependency.cmake )

IF ( WIN32 )
    MESSAGE( STATUS "Windows Platform for library")
    SET( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1 )
ENDIF()

MESSAGE( STATUS "Setting Done ")
# ----------------------------------------------- #



# ------ Create library ---------------- #
MESSAGE(STATUS "Create Library ")

IF ( BUILD_STATIC_LIBRARY )
    ADD_LIBRARY( ${PROJECT_NAME} STATIC ${SOURCE_DIR} )
    MESSAGE( STATUS "Build library as a static" )
ELSE ()
    ADD_LIBRARY( ${PROJECT_NAME} SHARED ${SOURCE_DIR} )
    MESSAGE( STATUS "Build library as a dynamic" )
ENDIF()

FOREACH( FOLDER ${FOLDER_DIR} )
    FILE( COPY ${FOLDER}/ DESTINATION ${INCLUDE_DIR}/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.h" )
ENDFOREACH()

IF ( CMAKE_BUILD_TYPE MATCHES Debug )
    SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ARC_DIR}
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${LIB_DIR}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}
    )
    MESSAGE( STATUS "Build library on Debug mode" )
ELSEIF ( CMAKE_BUILD_TYPE MATCHES Release )
    SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ARC_DIR}
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${LIB_DIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}
    )
    MESSAGE( STATUS "Build library on Release mode" )
ELSE()
    SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${ARC_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
    )
    MESSAGE( STATUS "Build library on Other mode" )
ENDIF()

MESSAGE( STATUS "Creating Done ")
# ----------------------------------------------- #



# ------ Set Options for Target Files ----------- #
MESSAGE(STATUS "Set Options for Target Files")

TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PUBLIC ${FOLDER_DIR} ${DEP_INCLUDE})
TARGET_LINK_DIRECTORIES( ${PROJECT_NAME} PUBLIC ${LIB_DIR})
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PUBLIC ${DEP_LIBS})
ADD_DEPENDENCIES( ${PROJECT_NAME} ${DEP_LIST})

MESSAGE(STATUS "Setting Done ")
# ----------------------------------------------- #



# ------ Create Demofile ------------------------ #
MESSAGE( STATUS "Create Demofile")

IF ( INSTALL_DEMO_FILE )
    ADD_SUBDIRECTORY( ${DEMOFILE_DIR} )
    MESSAGE( STATUS "Install Demofile that use MemoryProject" )
ELSE()
    MESSAGE( STATUS "Do not install ECSProject's Demofile" )
ENDIF()

MESSAGE( STATUS "Create Done")
# ----------------------------------------------- #