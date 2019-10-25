#
# CMake script for mathlib
#
project(mathlib C CXX)
file(GLOB_RECURSE SRCS *.c *.cpp *.cc *.h *.hh *.hpp)

add_library(mathlib STATIC ${SRCS})

set_target_properties(mathlib PROPERTIES CXX_STANDARD 17)
set_target_properties(mathlib PROPERTIES C_STANDARD 99)
target_include_directories(mathlib PRIVATE ${ENGINE} ${COMMON} ${ROOT} ${PUBLIC})
