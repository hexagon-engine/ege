set(SOURCES
	"GUIGameLoop.cpp"
	"GUIGameLoop.h"
	"GUIScreen.cpp"
	"GUIScreen.h"
	"Widget.cpp"
	"Widget.h"
)

ege_add_module(gui)
ege_depend_module(gui profiler)
ege_depend_module(gui resources)
ege_depend_module(gui syswindow)
