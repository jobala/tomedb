add_library(db OBJECT)

target_include_directories(db PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(db
PUBLIC
FILE_SET CXX_MODULES FILES
db.cpp
)
