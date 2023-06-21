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



#### Memory Pool ----------------------------------- #
MESSAGE(STATUS "Memory Project - Linking ...")

EXTERNALPROJECT_ADD(
    MemoryProject
    GIT_REPOSITORY GIT_REPOSITORY https://github.com/Winteradio/MemoryPool.git
    GIT_TAG "v2.1.2"

    PREFIX ${CMAKE_BINARY_DIR}/Prefix/MemoryProject

    UPDATE_COMMAND "" PATCH_COMMAND "" TEST_COMMAND "" INSTALL_COMMAND ""
    CMAKE_ARGS
        -DINCLUDE_DIR=${INCLUDE_DIR}
        -DLIB_DIR=${LIB_DIR}
        -DBIN_DIR=${BIN_DIR}
        -DARC_DIR=${ARC_DIR}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DBUILD_STATIC_LIBRARY=${BUILD_STATIC_LIBRARY}
        -DINSTALL_DEMO_FILE=OFF
        -DINSTALL_MEMORY_MANAGER=ON
)

LIST( APPEND DEP_INCLUDE ${INCLUDE_DIR} )
LIST( APPEND DEP_LIBS ${ARC_DIR}/MemoryProject.lib )
LIST( APPEND DEP_LIBS ${ARC_DIR}/LogProject.lib )
LIST( APPEND DEP_LIST MemoryProject)

MESSAGE(STATUS "Memory Project - Done")
#### MemoryPool ----------------------------------- #



#### UUID --------------------------------------- #
MESSAGE( STATUS "RPCRT4-UUID - Linking ... ")

SET( UUID_LIBS Rpcrt4.lib )

LIST( APPEND DEP_LIBS ${UUID_LIBS} )

MESSAGE( STATUS "RPCRT4-UUID - Done")
#### UUID --------------------------------------- #