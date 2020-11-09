/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/util/Vector.h>

namespace EGE
{

template<class TT, class CoordType>
class TileMap2D
{
public:
    typedef TT TileType;

    virtual TileType* getTile(EGE::Vector2<CoordType> vec) = 0;
    virtual TileType& ensureTile(EGE::Vector2<CoordType> vec) = 0;
};

}
