/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/util/ObjectMap.h>
#include <ege/util/Serializable.h>
#include <string>

namespace EGE
{

class GameplayObjectManager : public Serializable
{

public:
    virtual ~GameplayObjectManager() {}

    virtual bool reload();

    virtual bool clear() = 0;
    virtual bool load() = 0;

    std::string toString() const;

    virtual std::shared_ptr<ObjectMap> serialize() { return nullptr; }
    virtual void deserialize(std::shared_ptr<ObjectMap>) {}
};

}
