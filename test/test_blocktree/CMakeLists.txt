add_test_sources(
    TestBlocktree.cpp
)
set(TEST_NAME "TestBlocktree")

get_property(EXEC_NAME TARGET testblocks PROPERTY EXEC_NAME)
add_test(NAME ${TEST_NAME} COMMAND ${EXEC_NAME} "--gtest_filter=${TEST_NAME}.*")
