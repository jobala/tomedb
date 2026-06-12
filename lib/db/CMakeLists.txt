add_library(db OBJECT)

target_include_directories(db PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# TODO: make json lib private
target_link_libraries(db PUBLIC query_processor nlohmann_json::nlohmann_json)

target_sources(db
PUBLIC
FILE_SET CXX_MODULES FILES
db.cpp
collection.cpp
)
