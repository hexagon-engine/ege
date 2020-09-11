/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "ControlObject.h"

#include <ege/scene/SceneObject.h>
#include <ege/util/ObjectMap.h>
#include <memory>

namespace EGE
{

class Controller
{
public:
    Controller(std::shared_ptr<SceneObject> object)
    : m_object(object) {}

    virtual ~Controller() {}

    std::shared_ptr<SceneObject> getObject()
    {
        return m_object;
    }

    // handle requests from opposite side
    virtual void handleRequest(const ControlObject& data) = 0;

    // send request to opposite side
    virtual void sendRequest(const ControlObject& data) = 0;

private:
    std::shared_ptr<SceneObject> m_object;
};

}
