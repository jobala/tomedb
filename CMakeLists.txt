cmake_minimum_required(VERSION 4.2)

if(APPLE)
    # Point to Homebrew LLVM's clang++ which includes scanning tools
    set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
    set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm/bin/clang")
endif()

project(jack VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_GENERATOR "Ninja")


list (APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT CMAKE_CXX_COMPILER MATCHES "/usr/bin")
  add_link_options("-fuse-ld=lld" "-L/opt/homebrew/opt/llvm/lib/c++" "-Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++")
endif()

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

enable_testing()


add_subdirectory(lib/analyzer)
add_subdirectory(test)


add_executable(tomedb cmd/main.cpp)
target_link_libraries(tomedb PRIVATE analyzer)
