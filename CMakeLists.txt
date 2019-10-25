#
# CMakeLists.txt
#
# Main cmake script for uSource
#
cmake_minimum_required(VERSION 3.5)
project(dummy)

set(ROOT	${CMAKE_CURRENT_SOURCE_DIR})
set(ENGINE	${ROOT}/engine)
set(ENGINE_CLIENT ${ENGINE}/client)
set(ENGINE_SERVER ${ENGINE}/server)
set(ENGINE_PLATFORM ${ENGINE}/platform)
set(ENGINE_COMMON ${ENGINE}/common)
set(COMMON	${ROOT}/common)
set(MATHLIB ${ROOT}/mathlib)
set(PUBLIC	${ROOT}/public)
set(PMSHARED ${ROOT}/pm_shared)
set(GAME	${ROOT}/game)
set(SERVER	${GAME}/server)
set(SHARED	${GAME}/shared)
set(CLIENT	${GAME}/client)
set(LAUNCHER ${ROOT}/game_launch)

#option(AUTO_OPTIMIZE "Automatically selects the level of optimization to use with a host" ON)
option(USE_SSE "Enables the use of SSE/SSE2 optimization for x86 targets" ON)
option(USE_SSE41 "Enables the use of SSE4.1 optimizations for x86 targets" ON)
option(USE_AVX "Enables the use of AVX optimizations for x86 targets" ON)
option(USE_NEON "Enables the use of NEON vectorization for ARM-based targets" ON)

if(USE_SSE)
	add_definitions(-DUSE_SSE=1)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse")
endif(USE_SSE)

if(USE_SSE41)
	add_definitions(-DUSE_SSE41=1)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.1")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
endif(USE_SSE41)

if(USE_AVX)
	add_definitions(-DUSE_AVX=1)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
endif(USE_AVX)

if(USE_NEON)
	add_definitions(-DUSE_NEON=1)
endif(USE_NEON)


add_subdirectory(mathlib)
add_subdirectory(public)
add_subdirectory(game_launch)
