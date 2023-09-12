# ------ Set Options for Projects --------------- #
MESSAGE(STATUS "Set Options for Project")

## CMake minimum version required
CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

## Set CXX Compiler Version
SET(CMAKE_CXX_STANDARD 17)

SET( PROJECT_NAME "Demofile")
PROJECT( ${PROJECT_NAME} )

MESSAGE(STATUS "Setting Done")
# ----------------------------------------------- #


# ------ Set Files ------------------------------ #
MESSAGE(STATUS "Set Source File")

SET( MAIN_DIR ${CMAKE_SOURCE_DIR} )
SET( INTERFACE_DIR ${MAIN_DIR}/../Interface )
SET( MANAGER_DIR ${MAIN_DIR}/../Manager )
SET( UTILS_DIR ${MAIN_DIR}/../Utils )
SET( UUID_DIR ${MAIN_DIR}/../UUID )

## Source Files
LIST(APPEND SRCS
    ## Demo file
    ${MAIN_DIR}/main.cpp
    ${MAIN_DIR}/SystemList.h
    ${MAIN_DIR}/ComponentList.h
    ${MAIN_DIR}/NodeList.h

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

    ${UTILS_DIR}/ECSUtils.h

    ${UUID_DIR}/MyUUID.h ${UUID_DIR}/MyUUID.cpp
)

## Header Files
LIST(APPEND INCS 
    ${MAIN_DIR}
    ${INTERFACE_DIR}
    ${MANAGER_DIR}
    ${UTILS_DIR}
    ${UUID_DIR}
)

MESSAGE(STATUS "Setting Done ")
# ----------------------------------------------- #



# ------ Set Options for Target Files ----------- #
MESSAGE( STATUS "Add Executable by platform")

## Include Header Directories for Target Files
IF ( WIN32 )
    MESSAGE( STATUS "Windows Platform for demofile")
    ADD_EXECUTABLE( ${PROJECT_NAME} WIN32 ${SRCS} )

    SET( STATIC_LIB lib )
    SET( DYNAMIC_LIB dll )

ELSE ()
    MESSAGE( STATUS "Other Platform for demofile")
    ADD_EXECUTABLE( ${PROJECT_NAME} ${SRCS} )

    SET( STATIC_LIB a )
    SET( DYNAMIC_LIB so )

ENDIF()

MESSAGE( STATUS "Setting Done" )
# ----------------------------------------------- #



# ------ Set Executable file -------------------- #
MESSAGE(STATUS "Set Options for Target Files")

SET( CMAKE_BUILD_TYPE Debug CACHE STRING "Build Type" )

IF ( CMAKE_BUILD_TYPE MATCHES Debug )
    SET( EXE_DIR ${MAIN_DIR}/build/Debug )
    SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXE_DIR}
    )
    MESSAGE( STATUS "Build exe file on Debug mode" )
ELSEIF ( CMAKE_BUILD_TYPE MATCHES Release )
    SET( EXE_DIR ${MAIN_DIR}/build/Release )
    SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXE_DIR}
    )
    MESSAGE( STATUS "Build exe file on Release mode" )
ELSE()
    SET( EXE_DIR ${MAIN_DIR}/build )
    SET_TARGET_PROPERTIES( ${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${EXE_DIR}
    )
    MESSAGE( STATUS "Build exe file on Other mode" )
ENDIF()

MESSAGE( STATUS "Setting Done ")
# ----------------------------------------------- #



# ------ Download LogProject in Github ---------- #
MESSAGE( STATUS "Download LogProject" )

INCLUDE( ExternalProject )

SET( DEP_INCLUDE )
SET( DEP_LIBS )
SET( DEP_LIST )

SET( PREFIX_DIR ${MAIN_DIR}/build/Prefix )
SET( INCLUDE_DIR ${MAIN_DIR}/build/include )
SET( LIB_DIR ${MAIN_DIR}/build/lib )
SET( BIN_DIR ${EXE_DIR} )
SET( ARC_DIR ${MAIN_DIR}/build/arc )

EXTERNALPROJECT_ADD(
	MemoryProject_Demo
	GIT_REPOSITORY GIT_REPOSITORY https://github.com/Winteradio/MemoryPool.git
	GIT_TAG "v4.4.0"

	PREFIX ${PREFIX_DIR}/MemoryProject

	UPDATE_COMMAND "" PATCH_COMMAND "" TEST_COMMAND "" INSTALL_COMMAND ""
	CMAKE_ARGS 
        -DPREFIX_DIR=${PREFIX_DIR}
		-DINCLUDE_DIR=${INCLUDE_DIR}
		-DLIB_DIR=${LIB_DIR}
		-DBIN_DIR=${BIN_DIR}
        -DARC_DIR=${ARC_DIR}
		-DCMAKE_BUILD_TYPE=Debug
		-DBUILD_STATIC_LIBRARY=OFF
 		-DINSTALL_DEMO_FILE=OFF 
        -DINSTALL_MEMORY_MANAGER=ON
)
LIST( APPEND DEP_INCLUDE ${INCLUDE_DIR} )
LIST( APPEND DEP_LIST MemoryProject_Demo )
LIST( APPEND DEP_LIBS ${ARC_DIR}/MemoryProject.${STATIC_LIB} )
LIST( APPEND DEP_LIBS ${ARC_DIR}/LogProject.${STATIC_LIB} )

MESSAGE( STATUS "Downloading Done" )
# ----------------------------------------------- #



#### UUID --------------------------------------- #
MESSAGE( STATUS "RPCRT4-UUID - Linking ... ")

SET( UUID_LIBS Rpcrt4.lib )

LIST( APPEND DEP_LIBS ${UUID_LIBS} )

MESSAGE( STATUS "RPCRT4-UUID - Done")
#### UUID --------------------------------------- #



# ------ Link LogProject on Target  ------------- #
MESSAGE(STATUS "Set Options for Target Files")

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE} ${INCS})
TARGET_LINK_DIRECTORIES(${PROJECT_NAME} PUBLIC ${LIB_DIR})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC ${DEP_LIBS})
ADD_DEPENDENCIES(${PROJECT_NAME} ${DEP_LIST})

MESSAGE(STATUS "Setting Done ")
# ----------------------------------------------- #