
macro(ege_message level text)
	message("(EGE) ${level}: ${text}")
	if("${level}" STREQUAL "FATAL")
		message(FATAL "EGE Build System: A critical error has occurred and the build must stop")
	endif()
endmacro()

macro(ege_resources path)
	ege_message("INFO" "Adding RESOURCES: ${CMAKE_SOURCE_DIR}/${path} (installed in ${CMAKE_INSTALL_PREFIX}/${path})")
	install(DIRECTORY ${path} DESTINATION ".")
endmacro()
