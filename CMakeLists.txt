cmake_minimum_required(VERSION 3.10)
project(mc CXX)

macro(add_sources)
	file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")
	foreach(_src ${ARGN})
		if(_relPath)
			file(TO_NATIVE_PATH "${_relPath}/${_src}" nativePath)
		else()
			file(TO_NATIVE_PATH "${_src}" nativePath)
		endif()
		set_property(TARGET mc APPEND PROPERTY SRC_LIST "${nativePath}")
	endforeach()
endmacro()

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(EXECUTABLE_NAME "mc")

add_executable(${EXECUTABLE_NAME} "")
add_subdirectory(src)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(COMPILE_FLAGS "-Wall -fmax-errors=1 -ggdb -Werror")
	set(LINK_FLAGS "-Wall -fmax-errors=1 -ggdb -Werror -ldl")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(COMPILE_FLAGS "-Wall -ferror-limit=1 -g -Werror")
		set(LINK_FLAGS "-Wall -ferror-limit=1 -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(COMPILE_FLAGS "/W4 /MP /std:c++latest")
	set(LINK_FLAGS "/NODEFAULTLIB:msvcrtd.lib /std:c++latest")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINK_FLAGS}")

get_property(SRCS TARGET ${EXECUTABLE_NAME} PROPERTY SRC_LIST)
target_sources(${EXECUTABLE_NAME} PRIVATE ${SRCS})

target_include_directories(${EXECUTABLE_NAME} PRIVATE src)

if (UNIX)
	target_include_directories(${EXECUTABLE_NAME} PRIVATE /usr/include /usr/local/include)
endif (UNIX)

find_package(Threads REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} Threads::Threads)

find_package(SFML 2 REQUIRED COMPONENTS network audio graphics window system)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})

find_package(OpenGL REQUIRED)
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
