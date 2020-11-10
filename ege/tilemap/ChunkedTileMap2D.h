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

    ChunkType* getChunk(EGE::Vec2i chunkCoords)
    {
        return m_chunks[chunkCoords].get();
    }

    ChunkType& ensureChunk(EGE::Vec2i chunkCoords)
    {
        ChunkType* chunk = getChunk(chunkCoords);
        if(!chunk)
        {
            UniquePtr<ChunkType> generated = std::make_unique<ChunkType>();
            generateChunk(chunkCoords, *generated.get());
            ChunkType* ptr = generated.get();
            addChunk(chunkCoords, std::move(generated));
            return *ptr;
        }
        return *chunk;
    }

    void generateChunk(EGE::Vec2i chunkCoords, ChunkType& chunk)
    {
        if(m_generator)
            m_generator(chunkCoords, chunk);
        else
            initialize(chunk);
    }

    void initialize(ChunkType& chunk, const TileType& defaultTile = {})
    {
        for(Size x = 0; x < CSX; x++)
        for(Size y = 0; y < CSY; y++)
        {
            chunk.getTile(EGE::Vec2u(x, y)) = defaultTile;
        }
    }

    EGE::Vec2i getChunkCoords(EGE::Vec2i globalCoord)
    {
        return EGE::Vec2i(globalCoord / 16);
    }

    EGE::Vec2u getLocalCoords(EGE::Vec2i globalCoord)
    {
        return {globalCoord.x < 0 ? (unsigned)15 + (globalCoord.x + 1) % 16 : globalCoord.x % 16,
                globalCoord.y < 0 ? (unsigned)15 + (globalCoord.y + 1) % 16 : globalCoord.y % 16};
    }

    EGE::Vec2i getGlobalCoords(EGE::Vec2i chunkCoord, EGE::Vec2u localCoord);

    void setGenerator(std::function<void(EGE::Vec2i, ChunkType&)> func) { m_generator = func; }

    const auto begin() const { return m_chunks.begin(); }
    auto begin() { return m_chunks.begin(); }

    const auto end() const { return m_chunks.end(); }
    auto end() { return m_chunks.end(); }

    Size size() { return m_chunks.size(); }

private:
    void addChunk(EGE::Vec2i chunkCoords, UniquePtr<ChunkType> chunk)
    {
        m_chunks[chunkCoords] = std::move(chunk);
    }

    Map<EGE::Vec2i, UniquePtr<ChunkType>> m_chunks;
    std::function<void(EGE::Vec2i, ChunkType&)> m_generator;
};

}
