cmake_minimum_required(VERSION 3.19)
project("sfb_reader")


set(USE_GUI 1)

if (WIN32)
	add_definitions(-DUNICODE -D_UNICODE)	
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/build/win32>)
endif(WIN32)


if (USE_GUI EQUAL 1)
	find_package(wxWidgets COMPONENTS gl core base OPTIONAL_COMPONENTS net)
	include(${wxWidgets_USE_FILE})
	
	add_compile_definitions(USE_GUI)
	set(APPLICATION src/app/app.cpp src/app/gui.cpp src/sfb/sfb.cpp)
	add_executable(application WIN32 ${APPLICATION})
	target_link_libraries(application ${wxWidgets_LIBRARIES})

else()
	set(APPLICATION src/app/app.cpp src/sfb/sfb.cpp)
	add_executable(application ${APPLICATION})
endif()

