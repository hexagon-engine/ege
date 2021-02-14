set(SOURCES
	"AsyncHandler.cpp"
	"AsyncHandler.h"
	"AsyncLoop.cpp"
	"AsyncLoop.h"
	"AsyncTask.cpp"
	"AsyncTask.h"
	"ThreadSafeEventLoop.cpp"
	"ThreadSafeEventLoop.h"
)

ege_add_module(asyncLoop)
ege_depend_module(asyncLoop debug)
ege_depend_module(asyncLoop core)
ege_depend_module(asyncLoop sfml)
