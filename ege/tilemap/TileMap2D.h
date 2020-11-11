/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/util/Vector.h>
#include <cmath>

namespace EGE
{

template<class TT, class ST>
class TileMap2D
{
public:
    typedef TT TileType;
    typedef ST SizeType;

    const EGE::Vec2u getTileSize() const { return m_tileSize; }
    void setTileSize(const EGE::Vec2u& vec) { m_tileSize = vec; }

    virtual TileType* getTile(EGE::Vector2<SizeType> vec) = 0;
    virtual TileType& ensureTile(EGE::Vector2<SizeType> vec) = 0;

    EGE::Vec2i getTileAlignedPos(EGE::Vec2d vec)
    {
        return EGE::Vec2i(std::floor(std::floor(vec.x) / m_tileSize.x),
                          std::floor(std::floor(vec.y) / m_tileSize.y));
    }

private:
    EGE::Vec2u m_tileSize;
};

}
