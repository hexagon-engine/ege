set(SOURCES
	"Dump.cpp"
	"Dump.h"
	"Logger.cpp"
	"Logger.h"
	"Profiler.cpp"
	"Profiler.h"
	"ProfilerSectionStarter.h"
)

ege_add_module(debug)
ege_depend_module(debug util)
