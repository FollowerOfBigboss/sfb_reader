cmake_minimum_required(VERSION 3.19)
project("sfb_reader")

add_compile_definitions(WXUSINGDLL)

set(SOURCES 
	src/app/app.cpp 
	src/app/gui.cpp 
	src/sfb/sfb.cpp
)

if (WIN32)
	add_definitions(-DUNICODE -D_UNICODE)
	include_directories("3rdparty/wxwidgets/include/msvc")
	link_directories("3rdparty/wxwidgets/lib/win32")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/build/win32>)
endif(WIN32)


include_directories("3rdparty/wxwidgets/include")
add_executable(application ${SOURCES})