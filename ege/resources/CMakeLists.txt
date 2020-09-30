set(SOURCES
	"ResourceManager.cpp"
	"ResourceManager.h"
)

ege_add_module(resources)
ege_depend_module(resources debug)
ege_depend_module(resources sfml)
ege_depend_module(resources util)
