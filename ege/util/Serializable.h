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
    virtual ~Serializable() {}

    virtual SharedPtr<ObjectMap> serialize() = 0;
    virtual bool deserialize(SharedPtr<ObjectMap>) = 0;
};

}
