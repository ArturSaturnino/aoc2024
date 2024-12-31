cmake_minimum_required(VERSION 3.27)

project(aoc2024)
set(CMAKE_CXX_STANDARD 23)

FILE(GLOB Common common/*.h common/*.cpp)
add_library(aoc2024 STATIC ${Common})


FILE(GLOB fldrs ${CMAKE_SOURCE_DIR}/day*/day*.cpp)
foreach(fldr ${fldrs})
	get_filename_component(day ${fldr} NAME_WE ${CMAKE_SOURCE_DIR})
	FILE(GLOB _Files ${day}/*.cpp ${day}/*.h ${day}/*.txt)
	add_executable(${day} ${_Files})
	set_property(TARGET ${day} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/${day}")
	target_include_directories(${day} PRIVATE ${CMAKE_SOURCE_DIR})
	target_link_libraries(${day} PRIVATE aoc2024)
endforeach()

