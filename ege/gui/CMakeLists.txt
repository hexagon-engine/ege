set(SOURCES
	"Animatable.cpp"
	"Animatable.h"
	"Animation.cpp"
	"Animation.h"
	"AnimationEasingFunctions.cpp"
	"AnimationEasingFunctions.h"
	"Button.cpp"
	"Button.h"
	"GUIGameLoop.cpp"
	"GUIGameLoop.h"
	"GUIScreen.cpp"
	"GUIScreen.h"
	"Label.cpp"
	"Label.h"
	"Widget.cpp"
	"Widget.h"
)

ege_add_module(gui)
ege_depend_module(gui event)
ege_depend_module(gui profiler)
ege_depend_module(gui resources)
