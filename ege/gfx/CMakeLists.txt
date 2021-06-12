set(SOURCES
	"DefaultThemeRenderer.cpp"
	"DefaultThemeRenderer.h"
	"Renderer.cpp"
	"Renderer.h"
	"Renderable.cpp"
	"Renderable.h"
	"RenderStates.cpp"
	"RenderStates.h"
	"Text.cpp"
	"Text.h"
	"ThemeRenderer.cpp"
	"ThemeRenderer.h"
)

ege_add_module(gfx)
ege_depend_module(gfx core)
ege_depend_module(gfx resources)
target_link_libraries(ege-gfx PUBLIC sfml-graphics)
