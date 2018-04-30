cmake_minimum_required(VERSION 3.10)
project(testblocks CXX)

macro(add_test_sources)
    file (RELATIVE_PATH _relPath ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
    foreach(_src ${ARGN})
        if (_relPath)
            file(TO_NATIVE_PATH "${_relPath}/${_src}" _nativePath)
        else()
            file(TO_NATIVE_PATH ${_src} _nativePath)
        endif()
        set_property(TARGET testblocks APPEND PROPERTY SRC_LIST ${_nativePath})
    endforeach()
endmacro()

include(ExternalProject)
ExternalProject_Add(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG master
    SOURCE_DIR "${CMAKE_BINARY_DIR}/googletest/src"
    BINARY_DIR "${CMAKE_BINARY_DIR}/googletest/build"
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/googletest/install -DCMAKE_INSTALL_MESSAGE:STRING=LAZY
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(EXEC_NAME "testblocks")

link_directories(${CMAKE_BINARY_DIR}/googletest/install/lib)

add_executable(${EXEC_NAME} "Main.cpp")
add_dependencies(check testblocks)
add_dependencies(testblocks googletest)

#find_library(GTEST_LIBS NAMES gtest gmock HINTS ${CMAKE_BINARY_DIR}/googletest/install/lib)
target_link_libraries(${EXEC_NAME} libblocks gtest gmock)

set_property(TARGET testblocks PROPERTY EXEC_NAME ${EXEC_NAME}) 

target_include_directories(${EXEC_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src" "${CMAKE_BINARY_DIR}/googletest/install/include")

add_subdirectory(test_point)

get_property(SRCS TARGET ${EXEC_NAME} PROPERTY SRC_LIST)
target_sources(${EXEC_NAME} PRIVATE ${SRCS})
