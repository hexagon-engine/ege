/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "TileMap2D.h"

namespace EGE
{

template<class TT, size_t SX, size_t SY>
class FixedTileMap2D : public TileMap2D<TT, size_t>
{
public:
    virtual TT* getTile(EGE::Vec2s vec) override
    {
        if(vec.x >= SX || vec.y >= SY)
            return nullptr;
        return &m_tiles[vec.x][vec.y];
    }

    // The array is not dynamically resizable;
    // simply crash when we try do access something
    // out of bounds.
    virtual TT& ensureTile(EGE::Vec2s vec) override
    {
        TT* tile = getTile(vec);
        ASSERT(tile);
        return *tile;
    }

    EGE::Vec2s getSize()
    {
        return EGE::Vec2s(SX, SY);
    }

    void initialize(const TT& value = {})
    {
        for(size_t x = 0; x < SX; x++)
        for(size_t y = 0; y < SY; y++)
            m_tiles[x][y] = value;
    }

private:
    TT m_tiles[SX][SY];
};

}
