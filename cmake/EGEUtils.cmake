
macro(ege_message level text)
	string(ASCII 27 ESC)
	message("${ESC}[37mEGE_BUILD_SYSTEM ${ESC}[0;1;36m${level}${ESC}[0;35m : ${text}${ESC}[0m")
	if("${level}" STREQUAL "FATAL")
		message(FATAL "EGE Build System: A critical error has occurred and the build must stop")
	endif()
endmacro()
