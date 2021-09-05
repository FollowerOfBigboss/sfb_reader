cmake_minimum_required(VERSION 3.16.0)

project(sfb_editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

if (USEGUI)	
	add_compile_definitions(USEGUI)
	set(CMAKE_AUTOMOC ON)

	find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets REQUIRED)
	find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets REQUIRED)
	message(STATUS "Using Qt${QT_VERSION_MAJOR}")
	
	
	if (WIN32)
		set(QT_QMAKE_EXECUTABLE "${CMAKE_PREFIX_PATH}/bin/qmake.exe")
		message(STATUS "QMake executable found at ${QT_QMAKE_EXECUTABLE}" )
	endif()
	
	


	add_executable(sfbeditor
		source/sfb.cpp
		source/sfb.h
		source/main.cpp
		source/sfbgui.cpp
		source/sfbgui.h
	)
	
	target_link_libraries(sfbeditor Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core)
else()
	add_executable(sfbeditor
		source/sfb.cpp
		source/sfb.h
		source/main.cpp
	)
endif()
