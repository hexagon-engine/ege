/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "TileMap2D.h"

namespace EGE
{

template<class TT, Size SX, Size SY>
class FixedChunk : public TileMapChunk<TT, SX, SY>
{
public:
    TT& getTile(EGE::Vec2s vec)
    {
        if(vec.x >= SX || vec.y >= SY)
            CRASH();
        return m_tiles[vec.x][vec.y];
    }

    const TT& getTile(EGE::Vec2s vec) const
    {
        if(vec.x >= SX || vec.y >= SY)
            CRASH();
        return m_tiles[vec.x][vec.y];
    }

private:
    TT m_tiles[SX][SY];
};

}
