#
# project script for the launcher
#
# Jeremy Lorelli, Sep. 29, 2019
#
project(launcher CXX)

set(LAUNCHER_SRCS	game.cpp)

add_executable(launcher ${LAUNCHER_SRCS})
target_include_directories(launcher PRIVATE ${COMMON} ${PUBLIC})