add_test_sources(
    TestPoint.cpp
)
set(TEST_NAME "TestPoint")
get_property(EXEC_NAME TARGET testblocks PROPERTY EXEC_NAME)

add_test(NAME ${TEST_NAME} COMMAND ${EXEC_NAME} "--gtest_filter=${TEST_NAME}.*")
