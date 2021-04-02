cmake_minimum_required(VERSION 3.1.0)

project(sfb_editor)


include_directories("include")

if (WIN32)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS )
endif()

if (USEGUI)
	set(CMAKE_AUTOMOC ON)
	
	if(CMAKE_VERSION VERSION_LESS "3.7.0")
		set(CMAKE_INCLUDE_CURRENT_DIR ON)
	endif()

	add_compile_definitions(USEGUI)
	find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
	add_executable(sfbeditor
		source/sfb.cpp
		source/sfb.h
		source/main.cpp
		source/sfbgui.cpp
		source/sfbgui.h
	)
	target_link_libraries(sfbeditor Qt5::Widgets Qt5::Core)

else()
	add_executable(sfbeditor
		source/sfb.cpp
		source/sfb.h
		source/main.cpp
	)
endif()
