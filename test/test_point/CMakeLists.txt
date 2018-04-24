
set(TEST_NAME "testpoint")
set(SRCS Main.cpp
         TestPoint.cpp)

add_executable(${TEST_NAME} ${SRCS})
target_link_libraries(${TEST_NAME} libblocks gtest gmock)
target_include_directories(${TEST_NAME} PRIVATE "${CMAKE_BINARY_DIR}/googletest-src/googletest/include"
                                        "${CMAKE_BINARY_DIR}/googletest-src/googlemock/include"
                                        "${CMAKE_SOURCE_DIR}/src")

add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
"${CMAKE_BINARY_DIR}/googletest-build")

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
add_dependencies(check ${TEST_NAME})