/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Object.h"

#include <memory>

namespace EGE
{

class Serializable
{
public:
    virtual std::shared_ptr<Object> serialize() = 0;
    virtual void deserialize(std::shared_ptr<Object>) = 0;
};

}
