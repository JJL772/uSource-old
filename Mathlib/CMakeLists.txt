#
#
# CMakeLists.txt for Mathlib
#
#
project(mathlib C CXX)

set(MATHLIB_SOURCES	Mathlib.h
			Mathlib.cpp
)

add_library(mathlib STATIC ${MATHLIB_SOURCES})
