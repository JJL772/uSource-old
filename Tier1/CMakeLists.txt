#
#
# CMakeLists.txt for Tier1
#
#
project(tier1 C CXX)

set(TIER1_SOURCES	UtlList.h
			UtlArray.h
			Tier1.cpp
)

add_library(tier1 STATIC ${TIER1_SOURCES})
