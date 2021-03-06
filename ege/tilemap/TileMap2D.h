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

#include <ege/util/Vector.h>
#include <cmath>

namespace EGE
{

template<class TT, Size SX, Size SY>
class TileMapChunk
{
public:
    typedef TT TileType;
    static const Size SizeX = SX;
    static const Size SizeY = SY;

    virtual TT& getTile(EGE::Vec2s vec) = 0;
    virtual const TT& getTile(EGE::Vec2s vec) const = 0;
};

template<class TT, class ST, Size CSX = 16, Size CSY = 16>
class TileMap2D
{
public:
    typedef TT TileType;
    typedef ST SizeType;

    const EGE::Vec2u getTileSize() const { return m_tileSize; }
    void setTileSize(const EGE::Vec2u& vec) { m_tileSize = vec; }

    virtual TileType* getTile(EGE::Vector2<SizeType> vec) = 0;
    virtual TileType& ensureTile(EGE::Vector2<SizeType> vec) = 0;

    virtual const TileMapChunk<TT, CSX, CSY>* getChunk(EGE::Vec2i chunkCoords) const = 0;

    // Get pointer to chunk, if it exists.
    virtual TileMapChunk<TT, CSX, CSY>* getChunk(EGE::Vec2i chunkCoords) = 0;

    // Ensure that chunk exists, and return reference to it.
    virtual TileMapChunk<TT, CSX, CSY>& ensureChunk(EGE::Vec2i chunkCoords) = 0;

    EGE::Vec2s getChunkSize() { return {CSX, CSY}; }

    // Convert screen coordinates to tile coordinates.
    EGE::Vec2i getTileAlignedPos(EGE::Vec2d vec)
    {
        return EGE::Vec2i(std::floor(std::floor(vec.x) / m_tileSize.x),
                          std::floor(std::floor(vec.y) / m_tileSize.y));
    }

private:
    EGE::Vec2u m_tileSize;
};

}
