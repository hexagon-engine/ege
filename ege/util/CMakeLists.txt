set(SOURCES
	"Object.cpp"
	"Object.h"
	"ObjectMap.cpp"
	"ObjectMap.h"
	"ObjectString.cpp"
	"ObjectString.h"
	"Serializable.cpp"
	"Serializable.h"
)

ege_add_module(util)
ege_depend_module(util main)
