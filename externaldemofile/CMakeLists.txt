# ------ Set Options for Projects --------------- #
MESSAGE(STATUS "Set Options for Project")

## CMake minimum version required
CMAKE_MINIMUM_REQUIRED(VERSION 3.20)

## Set CXX Compiler Version
SET(CMAKE_CXX_STANDARD 17)

SET( DEMO_NAME "Demofile")
PROJECT( ${DEMO_NAME} )

MESSAGE(STATUS "Setting Done")
# ----------------------------------------------- #


# ------ Set Files ------------------------------ #
MESSAGE(STATUS "Set Source File")

IF ( NOT DEFINED DEMOFILE_DIR )
    SET( DEMOFILE_DIR ${CMAKE_SOURCE_DIR} )
ENDIF()

## Source Files
LIST(APPEND SRCS
    ## Demo file
    ${DEMOFILE_DIR}/main.cpp
    ${DEMOFILE_DIR}/SystemList.h
    ${DEMOFILE_DIR}/ComponentList.h
    ${DEMOFILE_DIR}/NodeList.h
)

## Header Files
LIST(APPEND INCS 
    ##Demo file
    ${DEMOFILE_DIR}
)

MESSAGE(STATUS "Setting Done ")
# ----------------------------------------------- #



# ------ Set Options for Target Files ----------- #
MESSAGE( STATUS "Add Executable by platform")

## Include Header Directories for Target Files
IF ( WIN32 )
    MESSAGE( STATUS "Windows Platform for demofile")
    ADD_EXECUTABLE( ${DEMO_NAME} WIN32 ${SRCS} )

    SET( STATIC_LIB lib )
    SET( DYNAMIC_LIB dll )

ELSE ()
    MESSAGE( STATUS "Other Platform for demofile")
    ADD_EXECUTABLE( ${DEMO_NAME} ${SRCS} )

    SET( STATIC_LIB a )
    SET( DYNAMIC_LIB so )

ENDIF()

MESSAGE( STATUS "Setting Done" )
# ----------------------------------------------- #



# ------ Set Executable file -------------------- #
MESSAGE(STATUS "Set Options for Target Files")

SET( CMAKE_BUILD_TYPE Debug CACHE STRING "Build Type" )

IF ( CMAKE_BUILD_TYPE MATCHES Debug )
    SET( EXE_DIR ${DEMOFILE_DIR}/build/Debug )
    SET_TARGET_PROPERTIES( ${DEMO_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${EXE_DIR}
    )
    MESSAGE( STATUS "Build exe file on Debug mode" )
ELSEIF ( CMAKE_BUILD_TYPE MATCHES Release )
    SET( EXE_DIR ${DEMOFILE_DIR}/build/Release )
    SET_TARGET_PROPERTIES( ${DEMO_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${EXE_DIR}
    )
    MESSAGE( STATUS "Build exe file on Release mode" )
ELSE()
    SET( EXE_DIR ${DEMOFILE_DIR}/build )
    SET_TARGET_PROPERTIES( ${DEMO_NAME} PROPERTIES
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

SET( DEMO_PREFIX_DIR ${DEMOFILE_DIR}/build/Prefix )
SET( DEMO_INCLUDE_DIR ${DEMOFILE_DIR}/build/include )
SET( DEMO_LIB_DIR ${DEMOFILE_DIR}/build/lib )
SET( DEMO_BIN_DIR ${EXE_DIR} )
SET( DEMO_ARC_DIR ${DEMOFILE_DIR}/build/arc )

EXTERNALPROJECT_ADD(
	ECSProject_demo
	GIT_REPOSITORY GIT_REPOSITORY https://github.com/Winteradio/ECS-with-Scene-and-Node.git
	GIT_TAG "v1.2.4"

	PREFIX ${DEMO_PREFIX_DIR}/ECSProject

	UPDATE_COMMAND "" PATCH_COMMAND "" TEST_COMMAND "" INSTALL_COMMAND ""
	CMAKE_ARGS 
        -DPREFIX_DIR=${DEMO_PREFIX_DIR}
		-DINCLUDE_DIR=${DEMO_INCLUDE_DIR}
		-DLIB_DIR=${DEMO_LIB_DIR}
		-DBIN_DIR=${DEMO_BIN_DIR}
        -DARC_DIR=${DEMO_ARC_DIR}
		-DCMAKE_BUILD_TYPE=Debug
		-DBUILD_STATIC_LIBRARY=OFF
 		-DINSTALL_DEMO_FILE=OFF 
)
LIST( APPEND DEP_INCLUDE ${DEMO_INCLUDE_DIR} )
LIST( APPEND DEP_LIBS ${DEMO_ARC_DIR}/ECSProject.${STATIC_LIB} )
LIST( APPEND DEP_LIBS ${DEMO_ARC_DIR}/MemoryProject.${STATIC_LIB} )
LIST( APPEND DEP_LIBS ${DEMO_ARC_DIR}/LogProject.${STATIC_LIB} )
LIST( APPEND DEP_LIST ECSProject_demo )

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
TARGET_LINK_DIRECTORIES(${PROJECT_NAME} PUBLIC ${DEMO_LIB_DIR})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC ${DEP_LIBS})
ADD_DEPENDENCIES(${PROJECT_NAME} ${DEP_LIST})

MESSAGE(STATUS "Setting Done ")
# ----------------------------------------------- #