/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#pragma once

#include "FixedChunk.h"
#include "TileMap2D.h"

namespace EGE
{

template<class TT, size_t SX, size_t SY>
class FixedTileMap2D : public TileMap2D<TT, size_t, SX, SY>
{
public:
    typedef TT TileType;
    typedef FixedChunk<TileType, SX, SY> ChunkType;

    virtual TT* getTile(EGE::Vec2s vec) override
    {
        if(vec.x >= SX || vec.y >= SY)
            return nullptr;
        return &m_tiles.getTile(vec);
    }

    // The array is not dynamically resizable;
    // simply crash when we try do access something
    // out of bounds.
    // (The chunk will automatically crash when
    // accessing tile out of range)
    virtual TT& ensureTile(EGE::Vec2s vec) override
    {
        TT* tile = &m_tiles.getTile(vec);
        return *tile;
    }

    EGE::Vec2s getSize()
    {
        return EGE::Vec2s(SX, SY);
    }

    virtual const ChunkType* getChunk(EGE::Vec2i chunkCoords) const
    {
        if(chunkCoords != EGE::Vec2i(0, 0))
            return nullptr;
        return &m_tiles;
    }

    // Get pointer to chunk, if it exists.
    virtual ChunkType* getChunk(EGE::Vec2i chunkCoords)
    {
        if(chunkCoords != EGE::Vec2i(0, 0))
            return nullptr;
        return &m_tiles;
    }

    // Ensure that chunk exists, and return reference to it.
    virtual ChunkType& ensureChunk(EGE::Vec2i chunkCoords)
    {
        if(chunkCoords != EGE::Vec2i(0, 0))
            CRASH();
        return m_tiles;
    }

    void initialize(const TT& value = {})
    {
        for(size_t x = 0; x < SX; x++)
        for(size_t y = 0; y < SY; y++)
            m_tiles.getTile({x, y}) = value;
    }

private:
    FixedChunk<TT, SX, SY> m_tiles;
};

}
