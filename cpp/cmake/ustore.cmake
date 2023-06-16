# ustore:
# https://github.com/unum-cloud/ustore/blob/main/CMakeLists.txt

include(ExternalProject)

set(REPOSITORY_BRANCH "main")
set(PREFIX_DIR ${CMAKE_BINARY_DIR}/_deps)
list(APPEND BUILD_ARGS "-DUSTORE_BUILD_ENGINE_UCSET=1" "-DUSTORE_BUILD_BUNDLES=1" "-DUSTORE_BUILD_TESTS=0" "-DUSTORE_BUILD_BENCHMARKS=0")

ExternalProject_Add(
    ustore_external

    GIT_REPOSITORY "https://github.com/unum-cloud/ustore"
    GIT_TAG "${REPOSITORY_BRANCH}"
    GIT_SHALLOW 1
    GIT_PROGRESS 0
    GIT_SUBMODULES ""
    
    PREFIX "${PREFIX_DIR}"
    DOWNLOAD_DIR "${PREFIX_DIR}/ustore-src"
    LOG_DIR "${PREFIX_DIR}/ustore-log"
    STAMP_DIR "${PREFIX_DIR}/ustore-stamp"
    TMP_DIR "${PREFIX_DIR}/ustore-tmp"
    SOURCE_DIR "${PREFIX_DIR}/ustore-src"
    INSTALL_DIR "${PREFIX_DIR}/ustore-install"
    BINARY_DIR "${PREFIX_DIR}/ustore-build"

    BUILD_ALWAYS 0
    UPDATE_COMMAND ""

    CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX:PATH=${PREFIX_DIR}/ustore-install
    -DCMAKE_INSTALL_LIBDIR=lib
    -DCMAKE_INSTALL_RPATH:PATH=<INSTALL_DIR>/lib
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_C_FLAGS=-Wno-maybe-uninitialized -Wno-implicit-fallthrough
    
    "${BUILD_ARGS}"
)

list(APPEND ustore_INCLUDE_DIRS ${PREFIX_DIR}/ustore-src/include ${PREFIX_DIR}/ustore-src/src)
file(MAKE_DIRECTORY ${ustore_INCLUDE_DIRS})
add_library(ustore STATIC IMPORTED)
set_property(TARGET ustore PROPERTY IMPORTED_LOCATION ${PREFIX_DIR}/ustore-build/build/lib/libustore_ucset_bundle.a)
set_property(TARGET ustore APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${ustore_INCLUDE_DIRS})
include_directories(${ustore_INCLUDE_DIRS})
add_dependencies(ustore ustore_external)
