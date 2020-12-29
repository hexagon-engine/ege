set(SOURCES
	"Renderer.cpp"
	"Renderer.h"
	"Renderable.cpp"
	"Renderable.h"
	"RenderStates.cpp"
	"RenderStates.h"
)

ege_add_module(gfx)
ege_depend_module(gfx loop)
ege_depend_module(gfx sfml)
ege_depend_module(gfx resources)
