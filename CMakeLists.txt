cmake_minimum_required(VERSION 2.8)
project(EGE)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/root")

### SFML ###

cmake_policy(SET CMP0074 NEW) # suppress SFML_ROOT warning
set(SFML_STATIC_LIBRARIES TRUE)
message("CG: SFML root is '${SFML_ROOT}'")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 COMPONENTS network audio graphics window system REQUIRED)
if(SFML_FOUND)
	message("CG: adding SFML as dependency")

	#target_include_directories(cgapi PUBLIC "${SFML_INCLUDE_DIR}")
	#target_link_libraries(cgapi PUBLIC ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

# add sfml extlibs
message("CG: sfml extlib path is ${SFML_ROOT}/${CG_EXTLIB_PATH}")
#link_directories("${SFML_ROOT}/${CG_EXTLIB_PATH}")

add_custom_target(tests
	COMMAND ${CMAKE_COMMAND} -E env "EGE_ROOT=${CMAKE_SOURCE_DIR}" ${CMAKE_SOURCE_DIR}/scripts/run-tests.sh
	USES_TERMINAL
)

macro(ege_link_sfml target)
	target_include_directories(${target} PUBLIC ${SFML_INCLUDE_DIR}/include)
	target_link_libraries(${target} PUBLIC ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endmacro()

macro(ege_add_module targetname)
	message("EGE: Adding MODULE: ${targetname}")
	add_library(${targetname} ${SOURCES})
	install(TARGETS ${targetname} ARCHIVE DESTINATION modules)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
		message("EGE: Adding MODULE TEST: ${targetname}")
		add_executable("test-${targetname}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp")
		target_link_libraries("test-${targetname}" ${targetname})
		install(TARGETS "test-${targetname}" RUNTIME DESTINATION tests)
	endif()
endmacro()

macro(ege_depend_module target dependtarget)
	message("EGE: Adding DEPENDENCY: ${target} >> ${dependtarget}")
	target_link_libraries(${target} ${dependtarget})
endmacro()

include_directories("${CMAKE_SOURCE_DIR}")

add_subdirectory(loop)
add_subdirectory(main)
add_subdirectory(profiler)
add_subdirectory(sfml)
add_subdirectory(sysevent)
add_subdirectory(syswindow)

