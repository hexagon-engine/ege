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
set(SFML_STATIC_LIBRARIES TRUE)

if(NOT DEFINED EGE_SFML_ROOT)
  ege_message("INFO" "Setting EGE_SFML_ROOT automatically")
  set(EGE_SFML_ROOT "${CMAKE_BINARY_DIR}/SFML")
endif()

set(SFML_ROOT ${EGE_SFML_ROOT})
ege_message("INFO" "SFML root is '${SFML_ROOT}'")
set(CMAKE_PREFIX_PATH "${SFML_ROOT}/build/root/lib/cmake/SFML")

cmake_policy(SET CMP0074 NEW) # suppress SFML_ROOT warning
# TODO: find_path is buggy?
find_package(SFML 2.5.1 COMPONENTS network audio graphics window system REQUIRED)

if(SFML_FOUND)
	ege_message("INFO" "Adding SFML as dependency")
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
        install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/res" DESTINATION ".")
    endif()
endmacro()

define_property(GLOBAL PROPERTY configured_modules BRIEF_DOCS "List of modules" FULL_DOCS "...")
# Initialize property
set_property(GLOBAL PROPERTY configured_modules "")

macro(ege_add_module targetname)
    set_property(GLOBAL APPEND PROPERTY configured_modules "${targetname}")
    set(target_name "ege-${targetname}")
    add_library(${target_name} STATIC ${SOURCES})
    target_compile_options(${target_name} PUBLIC -Wall -Wextra -Werror)

    # Sanitizers
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${target_name} PUBLIC -fsanitize=undefined -fsanitize=address)
        target_link_options(${target_name} PUBLIC -fsanitize=undefined -fsanitize=address)
    endif()

    # https://www.foonathan.net/2016/03/cmake-install/
    target_include_directories(${target_name} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)

    # Link mingw_stdthreads + stdlib on mingw
    if(${MINGW})
        target_link_libraries(${target_name} PUBLIC mingw_stdthreads)
        target_link_options(${target_name} PUBLIC -static-libgcc -static-libstdc++)
    endif()

    install(TARGETS ${target_name} EXPORT ege DESTINATION lib)
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/ege/" DESTINATION include/ege FILES_MATCHING PATTERN "*.h" PATTERN "*.inl")
    
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
        ege_add_all_tests_for(${targetname})
	endif()

endmacro()

macro(ege_depend_module target dependtarget)
	#ege_message("INFO", "Adding DEPENDENCY: ${target} >> ${dependtarget}")
	target_link_libraries(ege-${target} PUBLIC ege-${dependtarget})
endmacro()

add_subdirectory(ege)
add_subdirectory(examples)

install(FILES cmake/EGEConfig.cmake cmake/EGEUtils.cmake DESTINATION cmake)
install(EXPORT ege DESTINATION cmake)
get_property(modules GLOBAL PROPERTY configured_modules)
ege_message("INFO" "Configured modules: ${modules}")
