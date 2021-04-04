cmake_minimum_required(VERSION 3.1.0)

project(sfb_editor)

if (WIN32)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS )
endif()

if (USEGUI)
	set(CMAKE_AUTOMOC ON)
	
	if(CMAKE_VERSION VERSION_LESS "3.7.0")
		set(CMAKE_INCLUDE_CURRENT_DIR ON)
	endif()

	add_compile_definitions(USEGUI)
	find_package(Qt5 COMPONENTS Core Widgets Gui)
	
	if (Qt5_FOUND)
		message(STATUS "Qt5 found at ${Qt5_DIR}" )
	else()
		message("Couldn't find Qt5 please provide CMAKE_PREFIX_PATH in order to find Qt5")
		message("Example:")
		message("cmake .. -DUSEGUI=True -DCMAKE_PREFIX_PATH=D:/Qt/5.9.9/msvc2017_64\n")
		message(FATAL_ERROR "Aborting cmake because i couldn't find qt please follow steps at above. It may be helpful.")
	endif()

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
	target_link_libraries(sfbeditor Qt5::Widgets Qt5::Core Qt5::Gui)

else()
	add_executable(sfbeditor
		source/sfb.cpp
		source/sfb.h
		source/main.cpp
	)
endif()
