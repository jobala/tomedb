add_library(query_processor)

target_include_directories(query_processor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(query_processor
PUBLIC
FILE_SET CXX_MODULES FILES
statement.cpp
)
