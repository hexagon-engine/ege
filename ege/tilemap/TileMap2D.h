/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
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
