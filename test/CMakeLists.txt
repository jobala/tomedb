include(AddGoogleTest)

add_executable(query_processing_test query_processing_test.cpp)

target_link_libraries(query_processing_test PRIVATE gtest GTest::gtest_main  db)

include(GoogleTest)
gtest_discover_tests(query_processing_test)
