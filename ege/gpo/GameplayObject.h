/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/util/Serializable.h>

#include <string>

namespace EGE
{

class GameplayObject : public Serializable
{
    std::string m_id;
public:
    virtual ~GameplayObject() {}

    GameplayObject(std::string id)
    : m_id(id) {}

    std::string getId() const
    {
        return m_id;
    }
};

}
