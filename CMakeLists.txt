set(PIXLED_DRIVER_TEST OFF CACHE BOOL "Set to TRUE to compile this
component as a standalone esp-idf project and set up tests.")

if(PIXLED_DRIVER_TEST)
	cmake_minimum_required(VERSION 3.5)
	set(CXX_STANDARD 11)

	message("Building esp-pixled-driver standalone tests.")

	set(EXTRA_COMPONENT_DIRS test)

	include($ENV{IDF_PATH}/tools/cmake/project.cmake)

	project(pixled-driver-test CPP)
else()
	idf_component_register(
		SRCS "src/pixled_driver.cpp" "src/converters.cpp"
		INCLUDE_DIRS "include"
		)
endif()
