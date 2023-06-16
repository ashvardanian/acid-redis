# ucall:
# https://github.com/unum-cloud/ucall

include(FetchContent)

set(FETCHCONTENT_QUIET ON)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_Declare(
    ucall
    GIT_REPOSITORY https://github.com/unum-cloud/ucall
    GIT_TAG v0.5.1
    GIT_SHALLOW TRUE
)

set(UCALL_BUILD_EXAMPLES OFF)

FetchContent_MakeAvailable(ucall)
include_directories(${ucall_SOURCE_DIR}/include)