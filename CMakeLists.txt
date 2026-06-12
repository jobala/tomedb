cmake_minimum_required(VERSION 4.2)

if(APPLE)
    # Point to Homebrew LLVM's clang++ which includes scanning tools
    set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
    set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
  set(CMAKE_AR "/opt/homebrew/opt/llvm/bin/llvm-ar" CACHE FILEPATH "" FORCE)
  set(CMAKE_RANLIB "/opt/homebrew/opt/llvm/bin/llvm-ranlib" CACHE FILEPATH "" FORCE)
  set(CMAKE_CXX_COMPILER_AR "/opt/homebrew/opt/llvm/bin/llvm-ar" CACHE FILEPATH "" FORCE)
  set(CMAKE_C_COMPILER_AR "/opt/homebrew/opt/llvm/bin/llvm-ar" CACHE FILEPATH "" FORCE)
  set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> qc <TARGET> <LINK_FLAGS> <OBJECTS>")
  set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> <TARGET>")
  set(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> qc <TARGET> <LINK_FLAGS> <OBJECTS>")
  set(CMAKE_C_ARCHIVE_FINISH "<CMAKE_RANLIB> <TARGET>")
endif()

project(tome VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


list (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

option(ENABLE_SANITIZERS "Enable ASan and UBSan" OFF)
if(ENABLE_SANITIZERS)
  add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
  add_link_options(-fsanitize=address,undefined)
endif()

option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)
if(ENABLE_TSAN)
  add_compile_options(-fsanitize=thread -fno-omit-frame-pointer)
  add_link_options(-fsanitize=thread)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-Wall -Wextra -Wpedantic -Werror -Wno-missing-field-initializers)
endif()

include(add_json)
enable_testing()


add_subdirectory(lib/query_processor)
add_subdirectory(lib/db)
add_subdirectory(test)


add_executable(tomedb cmd/main.cpp)
target_link_libraries(tomedb PRIVATE  db)
