set(SOURCES
	"CameraObject.cpp"
	"CameraObject.h"
	"DummyObject.cpp"
	"DummyObject.h"
	"Object2D.cpp"
	"Object2D.h"
	"Scene.cpp"
	"Scene.h"
	"Scene2D.cpp"
	"Scene2D.h"
	"SceneObject.cpp"
	"SceneObject.h"
)

ege_add_module(scene)
ege_depend_module(scene gui)
