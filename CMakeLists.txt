cmake_minimum_required(VERSION 3.19)
project("sfb_reader")


set(USE_GUI 0)

if (USE_GUI EQUAL 1)
	add_compile_definitions(USE_GUI)
	add_compile_definitions(WXUSINGDLL)
	include_directories("3rdparty/wxwidgets/include")
	set(
	SOURCES 
		src/app/app.cpp 
		src/app/gui.cpp 
		src/sfb/sfb.cpp
	)

else()

	set(
	SOURCES 
		src/app/app.cpp 
		src/sfb/sfb.cpp
	)
endif()

if (WIN32)
	add_definitions(-DUNICODE -D_UNICODE)
	if (USE_GUI EQUAL 1)
		include_directories("3rdparty/wxwidgets/include/msvc")
		link_directories("3rdparty/wxwidgets/lib/win32")
	endif()
	
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/build/win32>)
endif(WIN32)


if (USE_GUI EQUAL 1 AND WIN32)
add_executable(application WIN32 ${SOURCES})
else()
add_executable(application ${SOURCES})
endif()