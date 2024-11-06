cmake_minimum_required (VERSION 3.18)

# get pntr
include(FetchContent)
# set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(pntr
  URL https://github.com/RobLoach/pntr/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(pntr)

# use wasi-sdk
set(CMAKE_SYSTEM_NAME WASI)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR wasm32)
set(triple wasm32-wasi)
if(NOT WASI_SDK_PREFIX)
    set(WASI_SDK_PREFIX /opt/wasi-sdk)
endif()
set(CMAKE_C_COMPILER ${WASI_SDK_PREFIX}/bin/clang${WASI_HOST_EXE_SUFFIX})
set(CMAKE_CXX_COMPILER ${WASI_SDK_PREFIX}/bin/clang++${WASI_HOST_EXE_SUFFIX})
set(CMAKE_ASM_COMPILER ${WASI_SDK_PREFIX}/bin/clang${WASI_HOST_EXE_SUFFIX})
set(CMAKE_AR ${WASI_SDK_PREFIX}/bin/llvm-ar${WASI_HOST_EXE_SUFFIX})
set(CMAKE_RANLIB ${WASI_SDK_PREFIX}/bin/llvm-ranlib${WASI_HOST_EXE_SUFFIX})
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER_TARGET ${triple})
set(CMAKE_ASM_COMPILER_TARGET ${triple})
set(HAVE_FLAG_SEARCH_PATHS_FIRST 0)
set(CMAKE_C_LINK_FLAGS "")
set(CMAKE_CXX_LINK_FLAGS "")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

# build test.wasm for pntr-demo
add_executable(test src/test.c)
target_link_libraries(test pntr)
add_custom_command(TARGET test POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:test>"
     "../../docs/pntr/$<TARGET_FILE_NAME:test>" 
  COMMENT "Copying to output directory")