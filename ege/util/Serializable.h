/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Converter.h"
#include "ObjectMap.h"

#include <memory>

namespace EGE
{

class Serializable
{
public:
    virtual std::shared_ptr<ObjectMap> serialize() = 0;
    virtual void deserialize(std::shared_ptr<ObjectMap>) = 0;
};

}
