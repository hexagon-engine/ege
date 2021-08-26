set(SOURCES
	"Camera.cpp"
	"Camera.h"
	"DummyObject2D.cpp"
	"DummyObject2D.h"
	"ObjectRenderer.cpp"
	"ObjectRenderer.h"
	"ParticleSystem2D.cpp"
	"ParticleSystem2D.h"
	"PhysicalParticleSystem.cpp"
	"PhysicalParticleSystem.h"
	"PhysicsBehaviour.cpp"
	"PhysicsBehaviour.h"
	"Plain2DCamera.cpp"
	"Plain2DCamera.h"
	"Scene.cpp"
	"Scene.h"
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
	"TexturedRenderer2D.cpp"
	"TexturedRenderer2D.h"
	"TilemapObject.cpp"
	"TilemapObject.h"
	"parts/CirclePart.cpp"
	"parts/CirclePart.h"
	"parts/Part.cpp"
	"parts/Part.h"
	"parts/PartStub.cpp"
	"parts/PartStub.h"
	"parts/PolygonPart.cpp"
	"parts/PolygonPart.h"
	"parts/RectanglePart.cpp"
	"parts/RectanglePart.h"
	"parts/TexturedPart.cpp"
	"parts/TexturedPart.h"
)

ege_add_module(scene)
ege_depend_module(scene gfx)
ege_depend_module(scene gpo)
ege_depend_module(scene gui)
ege_depend_module(scene tilemap)
ege_depend_module(scene util)

