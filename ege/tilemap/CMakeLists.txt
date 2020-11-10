set(SOURCES
	"ChunkedTileMap2D.cpp"
	"ChunkedTileMap2D.h"
	"FixedTileMap2D.cpp"
	"FixedTileMap2D.h"
	"TileMap2D.cpp"
	"TileMap2D.h"
)

ege_add_module(tilemap)
ege_depend_module(tilemap debug)
