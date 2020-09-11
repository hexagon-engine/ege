set(SOURCES
	"Controller.h"
	"Controller.cpp"
	"ControlObject.h"
	"ControlObject.cpp"
)

ege_add_module(controller)
ege_depend_module(controller loop) #?
ege_depend_module(controller scene)
ege_depend_module(controller util)
