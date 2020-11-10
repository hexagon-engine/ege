/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "TileMap2D.h"

#include <ege/util/Vector.h>

namespace EGE
{

template<class TT, Size SX, Size SY>
class Chunk
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

template<class TT, Size CSX, Size CSY>
class ChunkedTileMap2D : public TileMap2D<TT, int>
{
public:
    typedef TT TileType;
    typedef Chunk<TileType, CSX, CSY> ChunkType;

    virtual TileType* getTile(EGE::Vec2i vec) override
    {
        ChunkType* chunk = getChunk(getChunkCoords(vec));
        if(chunk)
            return &chunk->getTile(getLocalCoords(vec));
        return nullptr;
    }

    virtual TileType& ensureTile(EGE::Vec2i vec) override
    {
        return ensureChunk(getChunkCoords(vec)).getTile(getLocalCoords(vec));
    }

    const ChunkType* getChunk(EGE::Vec2i chunkCoords) const
    {
        return m_chunks[chunkCoords].get();
    }

    // Get pointer to chunk, if it exists.
    ChunkType* getChunk(EGE::Vec2i chunkCoords)
    {
        return m_chunks[chunkCoords].get();
    }

    // Ensure that chunk exists, and return reference to it.
    ChunkType& ensureChunk(EGE::Vec2i chunkCoords)
    {
        ChunkType* chunk = getChunk(chunkCoords);
        if(!chunk)
        {
            ChunkType& ptr = allocateChunk(chunkCoords);
            generateChunk(chunkCoords, *chunk);
            return ptr;
        }
        return *chunk;
    }

    // Generate chunk at specified position, if it doesn't
    // exist. It's almost equivalent to ensureChunk, except
    // that the function doesn't return a value.
    void generateChunk(EGE::Vec2i chunkCoords)
    {
        ensureChunk(chunkCoords);
    }

    // Force chunk to be regenerated / reinitialized and allocated if necessary.
    void regenerateChunk(EGE::Vec2i chunkCoords)
    {
        generateChunk(chunkCoords, getAndAllocateChunk(chunkCoords));
    }

    // Initialize chunk at specified position using specified tile.
    // Allocate chunk if it's necessary.
    void initialize(EGE::Vec2i chunkCoords, const TileType& defaultTile = {})
    {
        initialize(getAndAllocateChunk(chunkCoords), defaultTile);
    }

    EGE::Vec2i getChunkCoords(EGE::Vec2i globalCoord)
    {
        return EGE::Vec2i(globalCoord.x / CSX, globalCoord.y / CSY);
    }

    EGE::Vec2s getLocalCoords(EGE::Vec2i globalCoord)
    {
        return {globalCoord.x < 0 ? (unsigned)(CSX - 1) + (globalCoord.x + 1) % CSX : globalCoord.x % CSX,
                globalCoord.y < 0 ? (unsigned)(CSY - 1) + (globalCoord.y + 1) % CSY : globalCoord.y % CSY};
    }

    EGE::Vec2i getGlobalCoords(EGE::Vec2i chunkCoord, EGE::Vec2s localCoord);

    void setGenerator(std::function<void(EGE::Vec2i, ChunkType&)> func) { m_generator = func; }

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
            chunk.getTile(EGE::Vec2s(x, y)) = defaultTile;
        }
    }

    // Populate chunk using user-specified generator or default initializer.
    void generateChunk(EGE::Vec2i chunkCoords, ChunkType& chunk)
    {
        if(m_generator)
            m_generator(chunkCoords, chunk);
        else
            initialize(chunk);
    }

    // Add chunk to tile map without initializing it.
    ChunkType& allocateChunk(EGE::Vec2i chunkCoords)
    {
        UniquePtr<ChunkType> chunk = std::make_unique<ChunkType>();
        ChunkType& ptr = *chunk.get();
        addChunk(chunkCoords, std::move(chunk));
        return ptr;
    }

    // Return specified chunk and allocate it if it's necessary.
    ChunkType& getAndAllocateChunk(EGE::Vec2i chunkCoords)
    {
        ChunkType* chunk = getChunk(chunkCoords);
        if(!chunk)
        {
            chunk = &allocateChunk(chunkCoords);
        }
        return *chunk;
    }

    // Add pre-allocated chunk to tilemap.
    void addChunk(EGE::Vec2i chunkCoords, UniquePtr<ChunkType> chunk)
    {
        m_chunks[chunkCoords] = std::move(chunk);
    }

    Map<EGE::Vec2i, UniquePtr<ChunkType>> m_chunks;
    std::function<void(EGE::Vec2i, ChunkType&)> m_generator;
};

}
