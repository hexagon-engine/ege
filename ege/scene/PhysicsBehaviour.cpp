#include <ege/scene/PhysicsBehaviour.h>

namespace EGE
{

void GlobalPhysicsBehaviour::onUpdateObject(SceneObject& object)
{
    object.setMotion(object.getMotion() + getGravity());
}

bool PhysicsBehaviour::collisionResult(Vec3d)
{
    // TODO
    return false;
}

void PhysicsBehaviour::onUpdate()
{
    // TODO: Cache it somehow
    // TODO: What about deriving from objects?
    component().getOwner().forEachBehaviourOfType<GlobalPhysicsBehaviour>([&](auto& behaviour) {
        behaviour.onUpdateObject(component());
    });

    auto& object = component();
    object.setMotion(object.getMotion() + getGravity());
}

}
