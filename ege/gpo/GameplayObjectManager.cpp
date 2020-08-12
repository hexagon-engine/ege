/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "GameplayObjectManager.h"

namespace EGE
{

bool GameplayObjectManager::reload()
{
    return clear() && load();
}

std::string GameplayObjectManager::toString()
{
    return "EGE::GameplayObjectManager\n";
}

}
