#
# CMake project for Engine
#
project(engine C CXX)

file(GLOB_RECURSE SRC "${CMAKE_CURRENT_SOURCE_DIR}/common/" *.c *.cc *.cpp *.h *.hpp)

file(GLOB_RECURSE SRC_TMP "${CMAKE_CURRENT_SOURCE_DIR}/server/" *.c *.cc *.cpp *.h *.hpp)
list(APPEND SRC ${SRC_TMP})

if(NOT DEDICATED)
	file(GLOB_RECURSE SRC_TMP "${CMAKE_CURRENT_SOURCE_DIR}/client/" *.c *.cc *.cpp *.h *.hpp)
	list(APPEND SRC ${SRC_TMP})
endif(NOT DEDICATED)

if(LINUX)
endif(LINUX)

add_library(engine SHARED ${SRC})
