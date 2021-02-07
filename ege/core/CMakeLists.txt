set(SOURCES
	"Clock.cpp"
	"Clock.h"
	"Event.h"
	"EventCast.h"
	"EventHandler.h"
	"EventLoop.cpp"
	"EventLoop.h"
	"EventResult.h"
	"Timer.cpp"
	"Timer.h"
	"TimerEvent.h"
)

ege_add_module(loop)
ege_depend_module(loop debug util)
