set(SOURCES
	"Dump.cpp"
	"Dump.h"
)

ege_add_module(debug)
ege_depend_module(debug profiler)
ege_depend_module(debug util)
