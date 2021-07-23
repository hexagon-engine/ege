# FIXME: Make it a real CMake package

if(NOT IS_DIRECTORY ${EGE_LIB_ROOT})
	message(FATAL_ERROR "EGE_LIB_ROOT must be an existing directory (currently is set to '${EGE_LIB_ROOT}')")
endif()

include(${EGE_LIB_ROOT}/cmake/EGEUtils.cmake)

ege_message("INFO" "EGE lib root is '${EGE_LIB_ROOT}'")

# SFML preparation
set(CMAKE_MODULE_PATH "${EGE_LIB_ROOT}/cmake" ${CMAKE_MODULE_PATH})
set(SFML_STATIC_LIBRARIES TRUE)

if(NOT DEFINED EGE_BUILD_DIR)
  set(EGE_BUILD_DIR "${EGE_LIB_ROOT}/build")
endif()

if(NOT DEFINED EGE_SFML_ROOT)
  ege_message("INFO" "Setting EGE_SFML_ROOT automatically")
  set(EGE_SFML_ROOT "${EGE_BUILD_DIR}/SFML/build/root")
endif()

ege_message("INFO" "SFML root is '${EGE_SFML_ROOT}'")
set(SFML_ROOT ${EGE_SFML_ROOT})
cmake_policy(SET CMP0074 NEW) # suppress SFML_ROOT warning
# TODO: find_path is buggy?
find_package(SFML 2.4 COMPONENTS network audio graphics window system REQUIRED)

macro(ege_executable targetname sources modules)
	ege_message("INFO" "Adding EXECUTABLE: \"${targetname}\" (installed in ${CMAKE_INSTALL_PREFIX})")
	add_executable("${targetname}" ${sources})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/root")
	
	# add EGE modules
	foreach(module ${modules})
		target_link_libraries("${targetname}" PUBLIC "${EGE_LIB_ROOT}/build/root/modules/lib${module}.a")
	endforeach()
	target_include_directories("${targetname}" PUBLIC "${EGE_LIB_ROOT}")
	
	# add SFML
	if(SFML_FOUND)
		ege_message("INFO" "Adding SFML as dependency (SFML_INCLUDE_DIR=${SFML_INCLUDE_DIR})")
		target_link_libraries("${targetname}" PUBLIC sfml-graphics sfml-audio sfml-network)
		if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
			target_link_libraries("${targetname}" PUBLIC dl)
			target_link_libraries("${targetname}" PRIVATE GLEW)
		endif()
	endif()
	install(TARGETS "${targetname}" RUNTIME DESTINATION ".")
endmacro()

macro(ege_resources path)
	ege_message("INFO" "Adding RESOURCES: ${CMAKE_SOURCE_DIR}/${path} (installed in ${CMAKE_INSTALL_PREFIX}/${path})")
	install(DIRECTORY ${path} DESTINATION ".")
endmacro()

