#pragma once

#include <ege/scene/Scene.h>
#include <ege/scene/SceneObject.h>

namespace EGE
{

class PhysicsBehaviourBase
{
public:
    void setGravity(Vec3d gravity) { m_gravity = gravity; }
    Vec3d getGravity() const { return m_gravity; }

private:
    Vec3d m_gravity;
};

class GlobalPhysicsBehaviour : public PhysicsBehaviourBase, public Behaviour<Scene>
{
public:
    GlobalPhysicsBehaviour(Scene& object)
    : Behaviour<Scene>(object) {}

    // TODO: Fill it out!
    virtual void onUpdateObject(SceneObject&);
};

class PhysicsBehaviour : public PhysicsBehaviourBase, public Behaviour<SceneObject>
{
public:
    PhysicsBehaviour(SceneObject& object)
    : Behaviour<SceneObject>(object) {}

    EGE_ADD_TYPEID(override final)

    // TODO: Make this real collision result (point & new vector)
    // instead of just bool!
    virtual bool collisionResult(Vec3d offset);

    // TODO: Fill it out!
    virtual void onUpdate() override;

private:
    Vector<GlobalPhysicsBehaviour*> m_scenePhysics;
};

}
