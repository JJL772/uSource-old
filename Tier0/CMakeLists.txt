#
# 
# Project for Tier0, the platform abstraction library
#
#

project(Tier0)


set(TIER0_SOURCES	Tier0.h
			Tier0.cpp
			SystemInfo.h
			SystemInfo.cpp
)

add_library(Tier0 SHARED TIER0_SOURCES)

target_include_directories(Tier0 ./)

if(DEFINED PLATFORM_64BITS)
	target_compile_definitions(Tier0 PRIVATE -DPLATFORM_64BITS -D_X64_)
else()
	target_compile_definitions(Tier0 PRIVATE -DPLATFORM_32BITS -D_X86_ -D_I386_)
endif(DEFINED PLATFORM_64BITS)

if(DEFINED POSIX)
	target_compile_definitions(Tier0 PUBLIC -DLINUX -DPOSIX -D_POSIX -D_LINUX)
elseif(DEFINED WINDOWS)
	target_compile_definitions(Tier0 PUBLIC -DWIN32 -DWINDOWS -D_WINDOWS)
endif(DEFINED POSIX)

target_compile_definitions(Tier0 PRIVATE -DTIER0_EXPORT)

