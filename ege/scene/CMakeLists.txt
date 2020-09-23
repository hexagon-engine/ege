set(SOURCES
	"CameraObject.cpp"
	"CameraObject.h"
	"CameraObject2D.cpp"
	"CameraObject2D.h"
	"DummyObject2D.cpp"
	"DummyObject2D.h"
	"Renderer.cpp"
	"Renderer.h"
	"SceneObject2D.cpp"
	"SceneObject2D.h"
	"Scene.cpp"
	"Scene.h"
	"Scene2D.cpp"
	"Scene2D.h"
	"SceneObject.cpp"
	"SceneObject.h"
	"SceneWidget.cpp"
	"SceneWidget.h"
	"TexturedRenderer2D.cpp"
	"TexturedRenderer2D.h"
)

ege_add_module(scene)
ege_depend_module(scene gpo)
ege_depend_module(scene gui)
ege_depend_module(scene util)
