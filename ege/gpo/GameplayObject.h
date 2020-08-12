/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <string>

namespace EGE
{

class GameplayObject
{
    std::string m_id;
public:
    GameplayObject(std::string id)
    : m_id(id) {}

    std::string getId()
    {
        return m_id;
    }
};

}
