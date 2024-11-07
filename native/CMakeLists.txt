cmake_minimum_required (VERSION 3.18)

include(FetchContent)

string(TOLOWER ${CMAKE_HOST_SYSTEM_NAME} WAMR_BUILD_PLATFORM)
set (WAMR_BUILD_INTERP 1)
set (WAMR_BUILD_FAST_INTERP 0)
set (WAMR_BUILD_AOT 0)
set (WAMR_BUILD_LIBC_BUILTIN 1)
set (WAMR_BUILD_LIBC_WASI 1)
set (WAMR_BUILD_SIMD 1)

FetchContent_Declare(wamr
  URL https://github.com/bytecodealliance/wasm-micro-runtime/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(wamr)
include (${wamr_SOURCE_DIR}/build-scripts/runtime_lib.cmake)

# adjust WAMR_RUNTIME_LIB_SOURCE to use my WASI implementation
string(REPLACE "${wamr_SOURCE_DIR}/core/iwasm/libraries/libc-wasi/libc_wasi_wrapper.c" "${CMAKE_SOURCE_DIR}/src/libc_wasi_wrapper.c" WAMR_LIB_SRC "${WAMR_RUNTIME_LIB_SOURCE}")
add_library(vmlib ${WAMR_LIB_SRC})
target_include_directories(vmlib PRIVATE "${wamr_SOURCE_DIR}/core/iwasm/libraries/libc-wasi/")


add_executable(host src/host.c ${wamr_SOURCE_DIR}/core/shared/utils/uncommon/bh_read_file.c)
target_link_libraries(host vmlib)
target_include_directories(host PRIVATE ${wamr_SOURCE_DIR}/core/shared/utils/uncommon)

