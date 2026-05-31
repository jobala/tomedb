include(AddGoogleTest)

add_executable(analyzer_test analyzer_test.cpp)

target_link_libraries(analyzer_test PRIVATE gtest GTest::gtest_main analyzer)

include(GoogleTest)
gtest_discover_tests(analyzer_test)
