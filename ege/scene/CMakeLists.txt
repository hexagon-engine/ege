set(SOURCES
	"CameraObject.cpp"
	"CameraObject.h"
	"CameraObject2D.cpp"
	"CameraObject2D.h"
	"CirclePart.cpp"
	"CirclePart.h"
	"DummyObject2D.cpp"
	"DummyObject2D.h"
	"ObjectRenderer.cpp"
	"ObjectRenderer.h"
	"Part.cpp"
	"Part.h"
	"PartStub.cpp"
	"PartStub.h"
	"ParticleSystem2D.cpp"
	"ParticleSystem2D.h"
	"RectanglePart.cpp"
	"RectanglePart.h"
	"SceneObject2D.cpp"
	"SceneObject2D.h"
	"Scene.cpp"
	"Scene.h"
	"Scene2D.cpp"
	"Scene2D.h"
	"SceneLoader.cpp"
	"SceneLoader.h"
	"SceneObject.cpp"
	"SceneObject.h"
	"SceneObjectRegistry.cpp"
	"SceneObjectRegistry.h"
	"SceneObjectType.cpp"
	"SceneObjectType.h"
	"SceneWidget.cpp"
	"SceneWidget.h"
	"TexturedPart.cpp"
	"TexturedPart.h"
	"TexturedRenderer2D.cpp"
	"TexturedRenderer2D.h"
	"TilemapRenderer2D.cpp"
	"TilemapRenderer2D.h"
)

ege_add_module(scene)
ege_depend_module(scene controller)
ege_depend_module(scene gfx)
ege_depend_module(scene gpo)
ege_depend_module(scene gui)
ege_depend_module(scene tilemap)
ege_depend_module(scene util)

