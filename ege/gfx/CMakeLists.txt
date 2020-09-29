set(SOURCES
	"Renderer.h"
	"Renderer.cpp"
)

ege_add_module(gfx)
ege_depend_module(gfx loop)
ege_depend_module(gfx sfml)
ege_depend_module(gfx resources)
