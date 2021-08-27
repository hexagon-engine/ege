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

#include <ege/debug/Logger.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/Vector.h>
#include <functional>

namespace EGE
{

template<class TT, Size CSX, Size CSY>
class ChunkedTileMap2D : public TileMap2D<TT, int, CSX, CSY>
{
public:
    typedef TT TileType;
    typedef FixedChunk<TileType, CSX, CSY> ChunkType;

    virtual TileType* getTile(Vec2i vec) override
    {
        ChunkType* chunk = getChunk(getChunkCoords(vec));
        if(chunk)
            return &chunk->getTile(getLocalCoords(vec));
        return nullptr;
    }

    virtual TileType& ensureTile(Vec2i vec) override
    {
        auto lc = getLocalCoords(vec);
        return ensureChunk(getChunkCoords(vec)).getTile(lc);
    }

    const ChunkType* getChunk(Vec2i chunkCoords) const
    {
        auto it = m_chunks.find(chunkCoords);
        if(it == m_chunks.end())
            return nullptr;
        return it->second.get();
    }

    // Get pointer to chunk, if it exists.
    virtual ChunkType* getChunk(Vec2i chunkCoords)
    {
        return m_chunks[chunkCoords].get();
    }

    // Ensure that chunk exists, and return reference to it.
    virtual ChunkType& ensureChunk(Vec2i chunkCoords)
    {
        ChunkType* chunk = getChunk(chunkCoords);
        if(!chunk)
        {
            ChunkType& ptr = allocateChunk(chunkCoords);
            generateChunk(chunkCoords, ptr);
            return ptr;
        }
        return *chunk;
    }

    // Generate chunk at specified position, if it doesn't
    // exist. It's almost equivalent to ensureChunk, except
    // that the function doesn't return a value.
    void generateChunk(Vec2i chunkCoords)
    {
        ensureChunk(chunkCoords);
    }

    // Force chunk to be regenerated / reinitialized and allocated if necessary.
    void regenerateChunk(Vec2i chunkCoords)
    {
        generateChunk(chunkCoords, getAndAllocateChunk(chunkCoords));
    }

    // Initialize chunk at specified position using specified tile.
    // Allocate chunk if it's necessary.
    void initialize(Vec2i chunkCoords, const TileType& defaultTile = {})
    {
        initialize(getAndAllocateChunk(chunkCoords), defaultTile);
    }

    Vec2i getChunkCoords(Vec2i globalCoord)
    {
        return Vec2i(globalCoord.x / CSX, globalCoord.y / CSY);
    }

    Vec2s getLocalCoords(Vec2i globalCoord)
    {
        return {(globalCoord.x < 0 ? ((unsigned)(CSX - 1) + int(globalCoord.x + 1) % (int)CSX) : globalCoord.x % (int)CSX),
                (globalCoord.y < 0 ? ((unsigned)(CSY - 1) + int(globalCoord.y + 1) % (int)CSY) : globalCoord.y % (int)CSY)};
    }

    Vec2i getGlobalCoords(Vec2i chunkCoord, Vec2s localCoord);

    void setGenerator(std::function<void(Vec2i, ChunkType&)> func) { m_generator = func; }

    const auto begin() const { return m_chunks.begin(); }
    auto begin() { return m_chunks.begin(); }

    const auto end() const { return m_chunks.end(); }
    auto end() { return m_chunks.end(); }

    Size size() { return m_chunks.size(); }

private:
    // Initialize chunk with specified tile.
    void initialize(ChunkType& chunk, const TileType& defaultTile = {})
    {
        for(Size x = 0; x < CSX; x++)
        for(Size y = 0; y < CSY; y++)
        {
            chunk.getTile(Vec2s(x, y)) = defaultTile;
        }
    }

    // Populate chunk using user-specified generator or default initializer.
    void generateChunk(Vec2i chunkCoords, ChunkType& chunk)
    {
        if(m_generator)
            m_generator(chunkCoords, chunk);
        else
            initialize(chunk);
    }

    // Add chunk to tile map without initializing it.
    ChunkType& allocateChunk(Vec2i chunkCoords)
    {
        UniquePtr<ChunkType> chunk = makeUnique<ChunkType>();
        ChunkType& ptr = *chunk.get();
        addChunk(chunkCoords, std::move(chunk));
        return ptr;
    }

    // Return specified chunk and allocate it if it's necessary.
    ChunkType& getAndAllocateChunk(Vec2i chunkCoords)
    {
        ChunkType* chunk = getChunk(chunkCoords);
        if(!chunk)
        {
            chunk = &allocateChunk(chunkCoords);
        }
        return *chunk;
    }

    // Add pre-allocated chunk to tilemap.
    void addChunk(Vec2i chunkCoords, UniquePtr<ChunkType> chunk)
    {
        m_chunks[chunkCoords] = std::move(chunk);
    }

    Map<Vec2i, UniquePtr<ChunkType>> m_chunks;
    std::function<void(Vec2i, ChunkType&)> m_generator;
};

}
