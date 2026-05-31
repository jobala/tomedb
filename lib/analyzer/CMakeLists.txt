add_library(analyzer)

target_include_directories(analyzer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(analyzer
PUBLIC
FILE_SET CXX_MODULES FILES
parser.cpp
tokenizer.cpp
printer.cpp
)
