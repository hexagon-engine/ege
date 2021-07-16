set(SOURCES
	"AsyncTask.cpp"
	"AsyncTask.h"
	"Clock.cpp"
	"Clock.h"
	"DataManager.cpp"
	"DataManager.h"
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

ege_add_module(core)
ege_depend_module(core debug util)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD")
  target_link_libraries(ege-core PUBLIC pthread)
endif()
