add_library(db OBJECT)

target_include_directories(db PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(db PRIVATE query_processor)

target_sources(db
PUBLIC
FILE_SET CXX_MODULES FILES
db.cpp
collection.cpp
)
