set(SOURCES
	"DefaultSystemEventHandler.cpp"
	"DefaultSystemEventHandler.h"
	"SystemWindow.cpp"
	"SystemWindow.h"
)

ege_add_module(syswindow)
ege_depend_module(syswindow sysevent)
