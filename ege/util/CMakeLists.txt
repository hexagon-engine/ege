set(SOURCES
	"Converter.cpp"
	"Converter.h"
	"Object.cpp"
	"Object.h"
	"ObjectInt.cpp"
	"ObjectInt.h"
	"ObjectMap.cpp"
	"ObjectMap.h"
	"ObjectString.cpp"
	"ObjectString.h"
	"Serializable.cpp"
	"Serializable.h"
	"Vector.cpp"
	"Vector.h"
	"VectorOperations.cpp"
	"VectorOperations.h"
)

ege_add_module(util)
ege_depend_module(util main)
