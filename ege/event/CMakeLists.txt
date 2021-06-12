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
target_link_libraries(ege-event PRIVATE GLEW)

