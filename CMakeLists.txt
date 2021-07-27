cmake_minimum_required(VERSION 3.13)
project(EGE)

set(EGE_LIB_ROOT ${CMAKE_SOURCE_DIR})
include(${EGE_LIB_ROOT}/cmake/EGEUtils.cmake)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/root")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

### mingw_stdthreads
if(${MINGW})
    option(MINGW_STDTHREADS_GENERATE_STDHEADERS "" ON)
    add_subdirectory("${CMAKE_BINARY_DIR}/mingw-std-threads")
endif()

### SFML ###

# SFML preparation
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_BINARY_DIR}/SFML/cmake")
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
        set(target_name "ege-test-${targetname}-${testfile_only_name}")
        ege_message("INFO" "Adding test for module: ${target_name}")
        add_executable(${target_name} ${testfile})
        target_compile_options(${target_name} PUBLIC -Wall -Wextra)

        # Sanitizers
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            target_compile_options(${target_name} PUBLIC -fsanitize=undefined -fsanitize=address)
            target_link_options(${target_name} PUBLIC -fsanitize=undefined -fsanitize=address)
        endif()

        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/root")
        target_link_libraries(${target_name} PUBLIC "ege-${targetname}")
        install(TARGETS ${target_name} RUNTIME DESTINATION tests)
    endforeach()
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/res")
        ege_message("INFO" "MODULE TEST: Adding resource directory for ege-${targetname}")
        install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/res" DESTINATION ".")
    endif()
endmacro()

macro(ege_add_module targetname)
    ege_message("INFO" "Adding MODULE: ege-${targetname}")
    set(target_name "ege-${targetname}")
    add_library(${target_name} STATIC ${SOURCES})
    target_compile_options(${target_name} PUBLIC -Wall -Wextra -Werror)

    # Sanitizers
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${target_name} PUBLIC -fsanitize=undefined -fsanitize=address)
        target_link_options(${target_name} PUBLIC -fsanitize=undefined -fsanitize=address)
    endif()

    target_include_directories(${target_name} PUBLIC "${CMAKE_SOURCE_DIR}")

    # Link mingw_stdthreads + stdlib on mingw
    if(${MINGW})
        target_link_libraries(${target_name} PUBLIC mingw_stdthreads)
        target_link_options(${target_name} PUBLIC -static-libgcc -static-libstdc++)
    endif()

    install(TARGETS ${target_name} ARCHIVE DESTINATION modules)
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        ege_add_all_tests_for(${targetname})
	endif()

endmacro()

macro(ege_depend_module target dependtarget)
	#ege_message("INFO", "Adding DEPENDENCY: ${target} >> ${dependtarget}")
	target_link_libraries(ege-${target} PUBLIC ege-${dependtarget})
endmacro()

add_subdirectory(ege)

