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
ege_depend_module(event loop)
ege_depend_module(event sfml)
