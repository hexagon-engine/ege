set(SOURCES
	"ResourceManager.cpp"
	"ResourceManager.h"
)

ege_add_module(resources)
ege_depend_module(resources loop)
ege_depend_module(resources profiler)
ege_depend_module(resources sfml)
