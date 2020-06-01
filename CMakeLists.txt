#set(PIXLED_DRIVER_TEST OFF CACHE BOOL "Set to TRUE to compile this
#component as a standalone esp-idf project and set up tests.")
set(PIXLED_DRIVER_STANDALONE OFF CACHE BOOL "")

if(PIXLED_DRIVER_STANDALONE)
	cmake_minimum_required(VERSION 3.5)
	message("Building esp-pixled-driver standalone tests.")

	set(EXTRA_COMPONENT_DIRS test)

	include($ENV{IDF_PATH}/tools/cmake/project.cmake)

	project(pixled-driver-test)
else()
	idf_component_register(
		SRCS "src/pixled_driver.cpp"
		INCLUDE_DIRS "include"
		)
endif()
