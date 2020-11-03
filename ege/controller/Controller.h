/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Controllable.h"
#include "ControlObject.h"

#include <ege/util/ObjectMap.h>
#include <memory>

namespace EGE
{

class Controller
{
public:
    Controller(std::shared_ptr<Controllable> object)
    : m_object(object) {}

    virtual ~Controller() {}

    std::shared_ptr<Controllable> getObject()
    {
        return m_object;
    }

    // handle requests from opposite side
    virtual void handleRequest(const ControlObject& data) = 0;

    // send request to opposite side
    virtual void sendRequest(const ControlObject& data) = 0;

private:
    std::shared_ptr<Controllable> m_object;
};

}
