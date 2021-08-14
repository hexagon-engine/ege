set(SOURCES
	"AsyncTask.cpp"
	"AsyncTask.h"
	"BasicComponent.h"
	"Behaviour.h"
	"Clock.cpp"
	"Clock.h"
	"Component.cpp"
	"Component.h"
	"DataManager.cpp"
	"DataManager.h"
	"Event.h"
	"EventArray.h"
	"EventCast.h"
	"EventHandler.h"
	"EventResult.h"
	"EventTarget.cpp"
	"EventTarget.h"
	"LockingEventArray.h"
	"MainLoop.cpp"
	"MainLoop.h"
	"TickEvent.h"
	"Timer.cpp"
	"Timer.h"
	"TimerEvent.h"
)

ege_add_module(core)
ege_depend_module(core debug util)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
  target_link_libraries(ege-core PUBLIC pthread)
endif()
