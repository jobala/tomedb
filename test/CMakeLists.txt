include(AddGoogleTest)

add_executable(query_processing_test query_processing_test.cpp)
add_executable(query_execution_test query_execution_test.cpp)

target_link_libraries(query_processing_test PRIVATE gtest GTest::gtest_main  query_processor types db)
target_link_libraries(query_execution_test PRIVATE gtest GTest::gtest_main types db)

include(GoogleTest)
gtest_discover_tests(query_processing_test)
gtest_discover_tests(query_execution_test)
