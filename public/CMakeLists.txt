#
# CMake script for libpublic
#
# Jeremy Lorelli, Oct. 2019
#
project(public C CXX)

file(GLOB_RECURSE SRCS *.c *.cpp *.cc *.h *.hh *.hpp)

add_library(public STATIC ${SRCS})

set_target_properties(public PROPERTIES CXX_STANDARD 17)
set_target_properties(public PROPERTIES C_STANDARD 99)
target_include_directories(public PRIVATE ${ENGINE} ${COMMON} ${ROOT} ${PUBLIC})
