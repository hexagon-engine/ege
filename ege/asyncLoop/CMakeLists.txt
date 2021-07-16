set(SOURCES
	"ThreadSafeEventLoop.cpp"
	"ThreadSafeEventLoop.h"
)

ege_add_module(asyncLoop)
ege_depend_module(asyncLoop debug)
ege_depend_module(asyncLoop core)
target_link_libraries(ege-asyncLoop PUBLIC sfml-system)
