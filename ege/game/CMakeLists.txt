set(SOURCES
	"Game.cpp"
	"Game.h"
)

ege_add_module(game)
ege_depend_module(game gpo)
ege_depend_module(game gui)
ege_depend_module(game scene)
