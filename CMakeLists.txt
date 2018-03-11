cmake_minimum_required(VERSION 3.10)
project(mc CXX)

macro(add_sources)
	file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
	foreach(_src ${ARGN})
		if(_relPath)
			file(TO_NATIVE_PATH "${_relPath}/${_src}" _nativePath)
		else()
			file(TO_NATIVE_PATH ${_src} _nativePath)
		endif()
		set_property(TARGET mc APPEND PROPERTY SRC_LIST ${_nativePath})
	endforeach()
endmacro()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(EXECUTABLE_NAME "mc")

add_executable(${EXECUTABLE_NAME} "")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -fmax-errors=1 -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall -ferror-limit=1 -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_compile_options(${EXECUTABLE_NAME} PRIVATE /W4 /MP /std:c++latest)
endif()

add_subdirectory(src)

get_property(SRCS TARGET ${EXECUTABLE_NAME} PROPERTY SRC_LIST)
target_sources(${EXECUTABLE_NAME} PRIVATE ${SRCS})

target_include_directories(${EXECUTABLE_NAME} PRIVATE src)

#if (UNIX)
#	target_include_directories(${EXECUTABLE_NAME} PRIVATE /usr/include /usr/local/include)
#endif (UNIX)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${CMAKE_DL_LIBS})

find_package(Threads REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE Threads::Threads)

find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

find_package(GLM REQUIRED)
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE ${GLM_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
target_include_directories(${EXECUTABLE_NAME} SYSTEM PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${OPENGL_LIBRARIES})
