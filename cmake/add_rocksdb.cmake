
include(FetchContent)

set(WITH_GFLAGS OFF CACHE BOOL "" FORCE)
set(WITH_TESTS OFF CACHE BOOL "" FORCE)
set(WITH_TOOLS OFF CACHE BOOL "" FORCE)
set(WITH_BENCHMARK_TOOLS OFF CACHE BOOL "" FORCE)
set(WITH_SNAPPY ON CACHE BOOL "" FORCE)
set(WITH_LZ4 ON CACHE BOOL "" FORCE)
set(WITH_ZLIB ON CACHE BOOL "" FORCE)
set(PORTABLE ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    rocksdb
    GIT_REPOSITORY https://github.com/facebook/rocksdb.git
    GIT_TAG        v11.8.1
)

FetchContent_MakeAvailable(rocksdb)

set(LOGGER_SRC "${rocksdb_SOURCE_DIR}/util/stderr_logger.cc")
if(EXISTS "${LOGGER_SRC}")
    file(READ "${LOGGER_SRC}" LOGGER_CONTENT)
    # Check if cstdlib is already included to avoid appending it multiple times
    if(NOT LOGGER_CONTENT MATCHES "#include <cstdlib>")
        file(WRITE "${LOGGER_SRC}" "#include <cstdlib>\n${LOGGER_CONTENT}")
        message(STATUS "[RocksDB Patch] Successfully injected <cstdlib> into stderr_logger.cc")
    endif()
endif()

foreach(target rocksdb rocksdb-shared)
    if(TARGET ${target})
        target_compile_options(${target} PRIVATE
            -Wno-extra-semi
            -Wno-error
        )
    endif()
endforeach()

