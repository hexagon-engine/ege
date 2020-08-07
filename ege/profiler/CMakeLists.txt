set(SOURCES
	"Profiler.cpp"
	"Profiler.h"
	"ProfilerSectionStarter.h"
)

ege_add_module(profiler)
ege_depend_module(profiler main)
