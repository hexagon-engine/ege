cmake_minimum_required(VERSION 3.0)
project(EGE)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/root")

macro(ege_message level text)
	string(ASCII 27 ESC)
	message("${ESC}[37mEGE_BUILD_SYSTEM ${ESC}[0;1;36m${level}${ESC}[0;35m : ${text}${ESC}[0m")
endmacro()

### SFML ###

cmake_policy(SET CMP0074 NEW) # suppress SFML_ROOT warning
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
set(SFML_STATIC_LIBRARIES TRUE)
ege_message("INFO" "SFML root is '${EGE_SFML_ROOT}'")
set(SFML_ROOT ${EGE_SFML_ROOT})
find_package(SFML 2.4 COMPONENTS network audio graphics window system REQUIRED)
# TODO: find_path is buggy?
if(SFML_FOUND)
	ege_message("INFO" "Adding SFML as dependency (SFML_INCLUDE_DIR=${SFML_INCLUDE_DIR})")
endif()

add_custom_target(tests
	COMMAND ${CMAKE_COMMAND} -E env "EGE_ROOT=${CMAKE_SOURCE_DIR}" ${CMAKE_SOURCE_DIR}/scripts/run-tests.sh
	USES_TERMINAL
)

macro(ege_link_sfml target)
	target_include_directories(${target} PUBLIC ${SFML_INCLUDE_DIR})
	target_link_libraries(${target} PUBLIC ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endmacro()

macro(ege_add_module targetname)
	ege_message("INFO" "Adding MODULE: ${targetname}")
	add_library(${targetname} STATIC ${SOURCES})
	install(TARGETS ${targetname} ARCHIVE DESTINATION modules)
	##[[
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
		ege_message("INFO" "Adding MODULE TEST: ${targetname}")
		add_executable("test-${targetname}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/root")
		target_link_libraries("test-${targetname}" PUBLIC ${targetname})
		install(TARGETS "test-${targetname}" RUNTIME DESTINATION tests)
		if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/res")
			ege_message("INFO" "MODULE TEST: Adding resource directory for ${targetname}")
			install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/res" DESTINATION ".")
		endif()
	endif()
	#]]
endmacro()

macro(ege_depend_module target dependtarget)
	#ege_message("INFO", "Adding DEPENDENCY: ${target} >> ${dependtarget}")
	target_link_libraries(${target} PUBLIC ${dependtarget})
endmacro()

include_directories("${CMAKE_SOURCE_DIR}")

add_subdirectory(ege)

