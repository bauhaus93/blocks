cmake_minimum_required(VERSION 3.4.3)
project(mc CXX)

macro(add_sources)
	file (RELATIVE_PATH _relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}")

	foreach(_src ${ARGN})

		if(_relPath)
			set_property(TARGET mc APPEND PROPERTY SRC_LIST "${_relPath}/${_src}")
		else()
			set_property(TARGET mc APPEND PROPERTY SRC_LIST "${_src}")
		endif()

	endforeach()
endmacro()

add_executable(mc "")
add_subdirectory(src)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	set(COMPILE_FLAGS "-Wall -fmax-errors=1 -ggdb -Werror")
	set(LINK_FLAGS "-Wall -fmax-errors=1 -ggdb -Werror -ldl")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		set(COMPILE_FLAGS "-Wall -ferror-limit=1 -g -Werror")
		set(LINK_FLAGS "-Wall -ferror-limit=1 -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set(COMPILE_FLAGS "/W4 /MP")
	set(LINK_FLAGS "/NODEFAULTLIB:msvcrtd.lib")
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINK_FLAGS}")
set_property(TARGET mc PROPERTY CXX_STANDARD 17)

get_property(SRCS TARGET mc PROPERTY SRC_LIST)
target_sources(mc PRIVATE ${SRCS})

include_directories(src)

if (UNIX)
    include_directories(/usr/include)
endif (UNIX)
