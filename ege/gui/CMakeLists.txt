set(SOURCES
	"Animatable.cpp"
	"Animatable.h"
	"Animation.cpp"
	"Animation.h"
	"AnimationEasingFunctions.cpp"
	"AnimationEasingFunctions.h"
	"Button.cpp"
	"Button.h"
	"CheckBox.cpp"
	"CheckBox.h"
	"CompoundWidget.cpp"
	"CompoundWidget.h"
	"Frame.cpp"
	"Frame.h"
	"GameLoop.cpp"
	"GameLoop.h"
	"GUIGameLoop.cpp"
	"GUIGameLoop.h"
	"GUIResourceManager.cpp"
	"GUIResourceManager.h"
	"GUIScreen.cpp"
	"GUIScreen.h"
	"Label.cpp"
	"Label.h"
	"RadioButton.cpp"
	"RadioButton.h"
	"ScrollBar.cpp"
	"ScrollBar.h"
	"TextBox.cpp"
	"TextBox.h"
	"Widget.cpp"
	"Widget.h"
)

ege_add_module(gui)
ege_depend_module(gui asyncLoop)
ege_depend_module(gui debug)
ege_depend_module(gui event)
ege_depend_module(gui gfx)
ege_depend_module(gui resources)
