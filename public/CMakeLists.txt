#
# Build script for libpublic
#
# Jeremy Lorelli, Sep. 29, 2019
#
project(public C CXX)

set(PUBLIC_SRCS		crclib.h
					crclib.c
					crtlib.h
					crtlib.c
					mathlib.h
					mathlib.c
					matrixlib.c
					build.h)

add_library(public STATIC ${PUBLIC_SRCS})
target_include_directories(public PRIVATE ${COMMON} ${ENGINE})