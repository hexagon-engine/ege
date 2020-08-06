set(SOURCES
	"SystemEvent.h"
	"SystemEventHandler.cpp"
	"SystemEventHandler.h"
)

ege_add_module(sysevent)
ege_depend_module(sysevent loop)
ege_depend_module(sysevent main)
ege_depend_module(sysevent sfml)
