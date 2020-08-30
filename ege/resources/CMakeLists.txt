set(SOURCES
	"ResourceManager.cpp"
	"ResourceManager.h"
)

ege_add_module(resources)
ege_depend_module(resources main)
ege_depend_module(resources sfml)
ege_depend_module(resources util)
