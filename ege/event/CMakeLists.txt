set(SOURCES
	"DefaultSystemEventHandler.cpp"
	"DefaultSystemEventHandler.h"
	"KeybindManager.cpp"
	"KeybindManager.h"
	"SystemEvent.h"
	"SystemEventHandler.cpp"
	"SystemEventHandler.h"
	"SystemWindow.cpp"
	"SystemWindow.h"
)

ege_add_module(event)
ege_depend_module(event debug)
ege_depend_module(event core)
target_link_libraries(ege-event PUBLIC sfml-graphics)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	target_link_libraries(ege-event PRIVATE GLEW)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	target_include_directories(ege-event PUBLIC ${CMAKE_BINARY_DIR}/glew/include)
	target_link_directories(ege-event PUBLIC ${CMAKE_BINARY_DIR}/glew/lib)
	target_link_libraries(ege-event PUBLIC libglew32.a opengl32)
	target_compile_definitions(ege-event PUBLIC -DGLEW_STATIC)
endif()
