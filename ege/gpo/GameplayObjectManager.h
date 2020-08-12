/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <string>

namespace EGE
{

class GameplayObjectManager
{

public:
    virtual bool reload();

    virtual bool clear() = 0;
    virtual bool load() = 0;

    virtual std::string toString();
};

}
