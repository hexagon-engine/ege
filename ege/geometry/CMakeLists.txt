set(SOURCES
	"Line.cpp"
	"Line.h"
	"Point.cpp"
	"Point.h"
	"Polygon.cpp"
	"Polygon.h"
	"Ray.cpp"
	"Ray.h"
	"Shape.cpp"
	"Shape.h"
	"ShapeSet.cpp"
	"ShapeSet.h"
)

ege_add_module(geometry)
ege_depend_module(geometry util main)

