include(FetchContent)

set(WITH_GFLAGS OFF CACHE BOOL "" FORCE)
set(WITH_TESTS OFF CACHE BOOL "" FORCE)
set(WITH_TOOLS OFF CACHE BOOL "" FORCE)
set(WITH_BENCHMARK_TOOLS OFF CACHE BOOL "" FORCE)
set(WITH_SNAPPY ON CACHE BOOL "" FORCE)
set(WITH_LZ4 ON CACHE BOOL "" FORCE)
set(WITH_ZLIB ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    rocksdb
    GIT_REPOSITORY https://github.com/facebook/rocksdb.git
    GIT_TAG        v11.1.2
)

FetchContent_MakeAvailable(rocksdb)

target_compile_options(rocksdb PRIVATE
    -Wno-extra-semi
)
