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

    const EGE::Vec2u getTileSize() const { return m_tileSize; }
    void setTileSize(const EGE::Vec2u& vec) { m_tileSize = vec; }

    virtual TileType* getTile(EGE::Vector2<CoordType> vec) = 0;
    virtual TileType& ensureTile(EGE::Vector2<CoordType> vec) = 0;

private:
    EGE::Vec2u m_tileSize;
};

}
