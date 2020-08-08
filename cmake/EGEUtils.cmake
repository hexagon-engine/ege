
macro(ege_message level text)
	string(ASCII 27 ESC)
	message("${ESC}[37mEGE_BUILD_SYSTEM ${ESC}[0;1;36m${level}${ESC}[0;35m : ${text}${ESC}[0m")
	if("${level}" STREQUAL "FATAL")
		message(FATAL "EGE Build System: A critical error has occurred and the build must stop")
	endif()
endmacro()

macro(ege_link_sfml target)
	target_include_directories(${target} PUBLIC ${SFML_INCLUDE_DIR})
	target_link_libraries(${target} PUBLIC ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endmacro()
