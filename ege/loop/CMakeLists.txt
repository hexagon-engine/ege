set(SOURCES
	"Event.h"
	"EventCast.h"
	"EventHandler.h"
	"EventResult.h"
	"GameLoop.cpp"
	"GameLoop.h"
	"Time.h"
	"Timer.cpp"
	"Timer.h"
	"TimerEvent.h"
)

ege_add_module(loop)
ege_depend_module(loop main)
