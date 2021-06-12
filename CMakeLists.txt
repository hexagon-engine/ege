cmake_minimum_required(VERSION 3.11)
project(EGE)

set(EGE_LIB_ROOT ${CMAKE_SOURCE_DIR})
include(${EGE_LIB_ROOT}/cmake/EGEUtils.cmake)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/root")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### SFML ###

# SFML preparation
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${EGE_LIB_ROOT}/SFML/cmake")
set(SFML_STATIC_LIBRARIES TRUE)

if(NOT DEFINED EGE_SFML_ROOT)
  ege_message("INFO" "Setting EGE_SFML_ROOT automatically")
  set(EGE_SFML_ROOT "${CMAKE_BINARY_DIR}/SFML/build/root")
endif()

set(SFML_ROOT ${EGE_SFML_ROOT})
ege_message("INFO" "SFML root is '${SFML_ROOT}'")
ege_message("INFO" "CMAKE_BINARY_DIR is '${CMAKE_BINARY_DIR}'")

cmake_policy(SET CMP0074 NEW) # suppress SFML_ROOT warning
# TODO: find_path is buggy?
find_package(SFML 2.5.1 COMPONENTS network audio graphics window system REQUIRED)

if(SFML_FOUND)
	ege_message("INFO" "Adding SFML as dependency (SFML_INCLUDE_DIR=${SFML_INCLUDE_DIR})")
endif()

add_custom_target(tests
	COMMAND ${CMAKE_COMMAND} -E env "EGE_ROOT=${CMAKE_SOURCE_DIR}" ${CMAKE_SOURCE_DIR}/scripts/run-tests.sh
	USES_TERMINAL
)

macro(ege_add_all_tests_for targetname)
    file(GLOB TEST_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")

    foreach(testfile ${TEST_FILES})
        get_filename_component(testfile_only_name ${testfile} NAME_WE)
        ege_message("INFO" "Adding test for module: ege-test-${targetname}-${testfile_only_name}")
        add_executable("ege-test-${targetname}-${testfile_only_name}" ${testfile})
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/root")
        target_link_libraries("ege-test-${targetname}-${testfile_only_name}" PUBLIC "ege-${targetname}")
        install(TARGETS "ege-test-${targetname}-${testfile_only_name}" RUNTIME DESTINATION tests)
    endforeach()
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/res")
        ege_message("INFO" "MODULE TEST: Adding resource directory for ege-${targetname}")
        install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/res" DESTINATION ".")
    endif()
endmacro()

macro(ege_add_module targetname)
    ege_message("INFO" "Adding MODULE: ege-${targetname}")
    add_library("ege-${targetname}" STATIC ${SOURCES})
    install(TARGETS "ege-${targetname}" ARCHIVE DESTINATION modules)
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        ege_add_all_tests_for(${targetname})
	endif()

endmacro()

macro(ege_depend_module target dependtarget)
	#ege_message("INFO", "Adding DEPENDENCY: ${target} >> ${dependtarget}")
	target_link_libraries(ege-${target} PUBLIC ege-${dependtarget})
endmacro()

include_directories("${CMAKE_SOURCE_DIR}")

add_subdirectory(ege)

