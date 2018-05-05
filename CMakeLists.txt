cmake_minimum_required(VERSION 3.10)
project(blocks CXX)

macro(add_sources)
	file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
	foreach(_src ${ARGN})
		if(_relPath)
			file(TO_NATIVE_PATH "${_relPath}/${_src}" _nativePath)
		else()
			file(TO_NATIVE_PATH ${_src} _nativePath)
		endif()
		set_property(TARGET blks APPEND PROPERTY SRC_LIST ${_nativePath})
	endforeach()
endmacro()

macro(add_test_sources)
	file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
	foreach(_src ${ARGN})
		if(_relPath)
			file(TO_NATIVE_PATH "${_relPath}/${_src}" _nativePath)
		else()
			file(TO_NATIVE_PATH ${_src} _nativePath)
		endif()
		set_property(TARGET testblocks APPEND PROPERTY SRC_LIST ${_nativePath})
	endforeach()
endmacro()

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(EXEC_NAME "blocks")
set(EXEC_TEST_NAME "testblocks")
set(LIB_NAME "blks")

list(APPEND CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules")

add_executable(${EXEC_NAME} "src/Main.cpp")
add_executable(${EXEC_TEST_NAME} "src/MainTest.cpp")
add_library(${LIB_NAME} "")

target_link_libraries(${EXEC_NAME} ${LIB_NAME})
target_link_Libraries(${EXEC_TEST_NAME} ${LIB_NAME})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(${LIB_NAME} PRIVATE -Wall -fmax-errors=1 -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(${LIB_NAME} PRIVATE -Wall -ferror-limit=1 -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_compile_options(${LIB_NAME} PRIVATE /W4 /MP /std:c++latest)
endif()

add_subdirectory(src)

get_property(SRCS TARGET ${LIB_NAME} PROPERTY SRC_LIST)
target_sources(${LIB_NAME} PRIVATE ${SRCS})

get_property(TEST_SRCS TARGET ${EXEC_TEST_NAME} PROPERTY SRC_LIST)
target_sources(${EXEC_TEST_NAME} PRIVATE ${TEST_SRCS})

target_include_directories(${EXEC_NAME} PRIVATE src)
target_include_directories(${EXEC_TEST_NAME} PRIVATE src)
target_include_directories(${LIB_NAME} PRIVATE src)

target_link_libraries(${LIB_NAME} PRIVATE ${CMAKE_DL_LIBS})

find_package(Threads REQUIRED)
target_link_libraries(${LIB_NAME} PRIVATE Threads::Threads)

find_package(SFML 2 REQUIRED COMPONENTS window system)
target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${SFML_INCLUDE_DIR})
target_include_directories(${EXEC_NAME} SYSTEM PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(${LIB_NAME} PRIVATE ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

find_package(OpenGL REQUIRED)
target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(${LIB_NAME} PRIVATE ${OPENGL_LIBRARIES})

find_package(GTest REQUIRED COMPONENTS gtest gmock)
target_link_libraries(${EXEC_TEST_NAME} ${GTEST_LIBRARIES})
target_include_directories(${EXEC_TEST_NAME} SYSTEM PRIVATE ${GTEST_INCLUDE_DIR})

enable_testing()
gtest_discover_tests(${EXEC_TEST_NAME})

set(GLAD_ROOT_DIR ${CMAKE_BINARY_DIR}/glad)
include(ExternalProject)
ExternalProject_Add(glad
    GIT_REPOSITORY "https://github.com/Dav1dde/glad.git"
    GIT_TAG master
    SOURCE_DIR ${GLAD_ROOT_DIR}/src
    BINARY_DIR ${GLAD_ROOT_DIR}/build
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    UPDATE_COMMAND ""
)
add_dependencies(${LIB_NAME} glad)
target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${GLAD_ROOT_DIR}/build/include)
target_link_libraries(${LIB_NAME} PRIVATE ${GLAD_ROOT_DIR}/build/${CMAKE_STATIC_LIBRARY_PREFIX}glad${CMAKE_STATIC_LIBRARY_SUFFIX})

set(GLM_ROOT_DIR ${CMAKE_BINARY_DIR}/glm)
ExternalProject_Add(glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG master
    SOURCE_DIR ${GLM_ROOT_DIR}/src
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:Path=${GLM_ROOT_DIR}/install -DCMAKE_INSTALL_MESSAGE:String=LAZY
    TEST_COMMAND ""
    BUILD_COMMAND ""
    UPDATE_COMMAND ""
)
add_dependencies(${LIB_NAME} glm)
target_include_directories(${LIB_NAME} SYSTEM PRIVATE ${GLM_ROOT_DIR}/install/include)

