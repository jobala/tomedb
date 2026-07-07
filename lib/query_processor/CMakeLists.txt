add_library(query_processor)

target_include_directories(query_processor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(query_processor PRIVATE types)

target_sources(query_processor
PUBLIC
FILE_SET CXX_MODULES FILES
planner.cpp
query_processor.cpp
logical_plan.cpp
expr.cpp
)
