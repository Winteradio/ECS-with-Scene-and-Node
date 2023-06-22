# ------ Set for ExternalProject ---------------- #
MESSAGE( STATUS "Setting for ExternalProject")

INCLUDE( ExternalProject )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Variables for Dependency ----------- #
MESSAGE( STATUS "Setting Variables for Dependency")

SET( DEP_INCLUDE )
SET( DEP_LIBS )
SET( DEP_LIST )

MESSAGE( STATUS "Setting Done")
# ----------------------------------------------- #



# ------ Set Options for Target Files ----------- #
MESSAGE( STATUS "Add Executable by platform")

## Include Header Directories for Target Files
IF ( WIN32 )
    SET( STATIC_LIB lib )
    SET( DYNAMIC_LIB dll)

ELSE ()
    SET( STATIC_LIB a )
    SET( DYNAMIC_LIB so )

ENDIF()

MESSAGE( STATUS "Setting Done" )
# ----------------------------------------------- #



#### Memory Pool ----------------------------------- #
MESSAGE(STATUS "Memory Project - Linking ...")

EXTERNALPROJECT_ADD(
    MemoryProject
    GIT_REPOSITORY GIT_REPOSITORY https://github.com/Winteradio/MemoryPool.git
    GIT_TAG "v3.0.5"

    PREFIX ${PREFIX_DIR}/MemoryProject

    UPDATE_COMMAND "" PATCH_COMMAND "" TEST_COMMAND "" INSTALL_COMMAND ""
    CMAKE_ARGS
        -DPREFIX_DIR=${PREFIX_DIR}
        -DINCLUDE_DIR=${INCLUDE_DIR}
        -DLIB_DIR=${LIB_DIR}
        -DBIN_DIR=${BIN_DIR}
        -DARC_DIR=${ARC_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DBUILD_STATIC_LIBRARY=${BUILD_STATIC_LIBRARY}
        -DINSTALL_DEMO_FILE=${INSTALL_DEMO_FILE}
        -DINSTALL_MEMORY_MANAGER=ON
)

LIST( APPEND DEP_INCLUDE ${INCLUDE_DIR} )
LIST( APPEND DEP_LIST MemoryProject )
LIST( APPEND DEP_LIBS ${ARC_DIR}/MemoryProject.${STATIC_LIB} )
LIST( APPEND DEP_LIBS ${ARC_DIR}/LogProject.${STATIC_LIB} )

MESSAGE(STATUS "Memory Project - Done")
#### MemoryPool ----------------------------------- #



#### UUID --------------------------------------- #
MESSAGE( STATUS "RPCRT4-UUID - Linking ... ")

SET( UUID_LIBS Rpcrt4.lib )

LIST( APPEND DEP_LIBS ${UUID_LIBS} )

MESSAGE( STATUS "RPCRT4-UUID - Done")
#### UUID --------------------------------------- #