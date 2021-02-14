set(SOURCES
	"DefaultSystemEventHandler.cpp"
	"DefaultSystemEventHandler.h"
	"SystemEvent.h"
	"SystemEventHandler.cpp"
	"SystemEventHandler.h"
	"SystemWindow.cpp"
	"SystemWindow.h"
)

ege_add_module(event)
ege_depend_module(event debug)
ege_depend_module(event core)
ege_depend_module(event sfml)
target_link_libraries(ege-event PRIVATE GLEW)

