cmake_minimum_required(VERSION 3.0)
project(EGE)

set(EGE_LIB_ROOT ${CMAKE_SOURCE_DIR})
include(${EGE_LIB_ROOT}/cmake/EGEUtils.cmake)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/root")
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra")

### SFML ###

# SFML preparation
set(CMAKE_MODULE_PATH "${EGE_LIB_ROOT}/cmake" ${CMAKE_MODULE_PATH})
set(SFML_STATIC_LIBRARIES TRUE)

if(NOT DEFINED EGE_SFML_ROOT)
  set(EGE_SFML_ROOT "${CMAKE_BINARY_DIR}/SFML/build/root")
endif()

ege_message("INFO" "SFML root is '${EGE_SFML_ROOT}'")
set(SFML_ROOT ${EGE_SFML_ROOT})
cmake_policy(SET CMP0074 NEW) # suppress SFML_ROOT warning
# TODO: find_path is buggy?
find_package(SFML 2.4 COMPONENTS network audio graphics window system REQUIRED)

if(SFML_FOUND)
	ege_message("INFO" "Adding SFML as dependency (SFML_INCLUDE_DIR=${SFML_INCLUDE_DIR})")
endif()

add_custom_target(tests
	COMMAND ${CMAKE_COMMAND} -E env "EGE_ROOT=${CMAKE_SOURCE_DIR}" ${CMAKE_SOURCE_DIR}/scripts/run-tests.sh
	USES_TERMINAL
)

macro(ege_add_module targetname)
	ege_message("INFO" "Adding MODULE: ege-${targetname}")
	add_library("ege-${targetname}" STATIC ${SOURCES})
	install(TARGETS "ege-${targetname}" ARCHIVE DESTINATION modules)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
		ege_message("INFO" "Adding MODULE TEST: ege-${targetname}")
		add_executable("ege-test-${targetname}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/root")
		target_link_libraries("ege-test-${targetname}" PUBLIC "ege-${targetname}")
		install(TARGETS "ege-test-${targetname}" RUNTIME DESTINATION tests)
		if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/res")
			ege_message("INFO" "MODULE TEST: Adding resource directory for ege-${targetname}")
			install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/res" DESTINATION ".")
		endif()
	endif()
endmacro()

macro(ege_depend_module target dependtarget)
	#ege_message("INFO", "Adding DEPENDENCY: ${target} >> ${dependtarget}")
	target_link_libraries("ege-${target}" PUBLIC "ege-${dependtarget}")
endmacro()

include_directories("${CMAKE_SOURCE_DIR}")

add_subdirectory(ege)

