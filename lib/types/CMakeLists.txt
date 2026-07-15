add_library(types)

target_include_directories(types PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(types PRIVATE nlohmann_json::nlohmann_json)

target_sources(types 
PUBLIC
FILE_SET CXX_MODULES FILES
types.cpp
uuid.cpp
)
