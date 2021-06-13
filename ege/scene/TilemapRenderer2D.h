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

#include "ObjectRenderer.h"
#include "Scene.h"
#include "SceneObject.h"

#include <ege/gfx/Renderer.h>
#include <ege/resources/Texture.h>
#include <ege/tilemap/TileMap2D.h>
#include <ege/util.h>

namespace EGE
{

template<class TMap>
class TilemapRenderer2D : public ObjectRenderer
{
public:
    TilemapRenderer2D(SceneObject& sceneObject, SharedPtr<TMap> tilemap)
    : ObjectRenderer(sceneObject), m_tileMap(tilemap) { setLayerCount(1); }

    void setAtlasTextureName(std::string name, Size layer = 0)
    {
        ASSERT(layer < m_layerCount);
        if(m_atlasNames[layer] != name)
        {
            m_atlasNames[layer] = name;
            setGeometryNeedUpdate();
        }
    }

    struct AtlasInfo
    {
        Vec2i texCoords;
        int rotation = 0; // 90*x deg
    };

    // Function void map(const TileType& tile, EGE::Vector2<MaxInt> tilePos, EGE::Size layer, AtlasInfo& info);
    typedef std::function<void(const typename TMap::TileType&, Vector2<MaxInt>, Size, AtlasInfo&)> AtlasMapper;

    // The function maps tile type (in tilemap) to atlas coords to render (in pixels).
    void setTileAtlasMapper(AtlasMapper mapper) { m_tileMapper = mapper; }

    // Use ensureTile() instead of getTile(). The tilemap will be resized
    // if possible and necessary. DO NOT USE IT WITH FIXED TILEMAPS, otherwise
    // assertion will be triggered.
    void setUseEnsure(bool ensure = true) { m_useEnsure = ensure; }

    // Useful for shadows.
    void setLayerCount(Size lc)
    {
        ASSERT(lc > 0);
        m_layerCount = lc;
        m_atlasNames.resize(lc);
        m_atlasses.resize(lc);
        setGeometryNeedUpdate();
    }

    virtual void renderLayer(Size layer, Vec2d objPos, Vector2<MaxInt> beginChunk, Vector2<MaxInt> endChunk, Renderer& renderer) const
    {
        sf::VertexArray vertexes(sf::Quads);

        Vec2u tileSize = m_tileMap->getTileSize();
        Vec2s chunkSize = m_tileMap->getChunkSize();

        ASSERT(tileSize.x != 0);
        ASSERT(tileSize.y != 0);
        ASSERT(chunkSize.x != 0);
        ASSERT(chunkSize.y != 0);

        Size arraySize = (endChunk.x - beginChunk.x + 1) * (endChunk.y - beginChunk.y + 1) * chunkSize.x * chunkSize.y * 4;
        vertexes.resize(arraySize);
        Size index = 0;
        Size realIndex;

        // TODO: calculate and reserve required space
        // TODO: don't check tiles outside the bounds
        // TODO: allow checking bounds inside tilemap if it's applicable
        for(MaxInt cx = beginChunk.x; cx <= endChunk.x; cx++)
        {
            for(MaxInt cy = beginChunk.y; cy <= endChunk.y; cy++)
            {
                const typename TMap::ChunkType* chunk = (m_useEnsure
                                                                ? &m_tileMap->ensureChunk({(typename TMap::SizeType)cx, (typename TMap::SizeType)cy})
                                                                : m_tileMap->getChunk({(typename TMap::SizeType)cx, (typename TMap::SizeType)cy})
                                                            );
                if(!chunk)
                    continue;

                for(Size x = 0; x < chunkSize.x; x++)
                {
                    for(Size y = 0; y < chunkSize.y; y++)
                    {
                        const typename TMap::TileType& tile = chunk->getTile({x, y});

                        MaxInt vx = cx * (MaxInt)chunkSize.x + (MaxInt)x;
                        MaxInt vy = cy * (MaxInt)chunkSize.y + (MaxInt)y;

                        AtlasInfo info;
                        m_tileMapper(tile, {vx, vy}, layer, info);

                        // tex coords
                        Size index2 = index;
                        {
                            sf::Vertex& vertex = vertexes[index2++];
                            vertex.texCoords.x = info.texCoords.x;
                            vertex.texCoords.y = info.texCoords.y;
                        }

                        {
                            sf::Vertex& vertex = vertexes[index2++];
                            vertex.texCoords.x = info.texCoords.x + tileSize.x - 1;
                            vertex.texCoords.y = info.texCoords.y;
                        }

                        {
                            sf::Vertex& vertex = vertexes[index2++];
                            vertex.texCoords.x = info.texCoords.x + tileSize.x - 1;
                            vertex.texCoords.y = info.texCoords.y + tileSize.y - 1;
                        }

                        {
                            sf::Vertex& vertex = vertexes[index2++];
                            vertex.texCoords.x = info.texCoords.x;
                            vertex.texCoords.y = info.texCoords.y + tileSize.y - 1;
                        }

                        // position & color
                        {
                            realIndex = (index / 4 * 4) + (index + info.rotation) % 4;
                            sf::Vertex& vertex = vertexes[realIndex];
                            vertex.position.x = vx * tileSize.x + objPos.x;
                            vertex.position.y = vy * tileSize.y + objPos.y;

                            // color
                            vertex.color.r = 255;
                            vertex.color.g = 255;
                            vertex.color.b = 255;
                            vertex.color.a = 255;

                            index++;
                        }

                        {
                            realIndex = (index / 4 * 4) + (index + info.rotation) % 4;
                            sf::Vertex& vertex = vertexes[realIndex];
                            vertex.position.x = (vx + 1) * tileSize.x + objPos.x;
                            vertex.position.y = vy * tileSize.y + objPos.y;

                            // color
                            vertex.color.r = 255;
                            vertex.color.g = 255;
                            vertex.color.b = 255;
                            vertex.color.a = 255;

                            index++;
                        }

                        {
                            realIndex = (index / 4 * 4) + (index + info.rotation) % 4;
                            sf::Vertex& vertex = vertexes[realIndex];
                            vertex.position.x = (vx + 1) * tileSize.x + objPos.x;
                            vertex.position.y = (vy + 1) * tileSize.y + objPos.y;

                            // color
                            vertex.color.r = 255;
                            vertex.color.g = 255;
                            vertex.color.b = 255;
                            vertex.color.a = 255;

                            index++;
                        }

                        {
                            realIndex = (index / 4 * 4) + (index + info.rotation) % 4;
                            sf::Vertex& vertex = vertexes[realIndex];
                            vertex.position.x = vx * tileSize.x + objPos.x;
                            vertex.position.y = (vy + 1) * tileSize.y + objPos.y;

                            // color
                            vertex.color.r = 255;
                            vertex.color.g = 255;
                            vertex.color.b = 255;
                            vertex.color.a = 255;

                            index++;
                        }
                    }
                }
            }
        }

        // TODO: use triangles?
        auto texture = m_atlasses[layer];
        ASSERT(texture);
        RenderStates newStates = renderer.getStates();
        newStates.sfStates().texture = &texture->getTexture();
        renderer.getTarget().draw(vertexes, newStates.sfStates());
    }

    virtual void render(Renderer& renderer) const override
    {
        // TODO: tilemap render cache (it should go to updateGeometry)
        ASSERT(m_tileMapper);

        Vec2u tileSize = m_tileMap->getTileSize();
        Vec2s chunkSize = m_tileMap->getChunkSize();

        ASSERT(tileSize.x != 0);
        ASSERT(tileSize.y != 0);
        ASSERT(chunkSize.x != 0);
        ASSERT(chunkSize.y != 0);
        Scene& scene = (Scene&)m_sceneObject.getOwner();

        // TODO: allow setting tilemap bounds
        Vec2d beginCoord = scene.mapToSceneCoords(renderer, {0, 0}).toVec2d();
        auto targetSize = renderer.getTarget().getSize();
        Vec2d endCoord = scene.mapToSceneCoords(renderer, {static_cast<double>(targetSize.x), static_cast<double>(targetSize.y)}).toVec2d();
        Vec2d objPos = m_sceneObject.getPosition().toVec2d();

        Vector2<MaxInt> beginChunk = {
            (MaxInt)((beginCoord.x - objPos.x) / ((MaxInt)tileSize.x * chunkSize.x) - 1),
            (MaxInt)((beginCoord.y - objPos.y) / ((MaxInt)tileSize.y * chunkSize.y) - 1)
        };
        Vector2<MaxInt> endChunk = {
            (MaxInt)((endCoord.x - objPos.x) / ((MaxInt)tileSize.x * chunkSize.x) + 1),
            (MaxInt)((endCoord.y - objPos.y) / ((MaxInt)tileSize.y * chunkSize.y) + 1)
        };

        for(Size s = 0; s < m_layerCount; s++)
            renderLayer(s, objPos, beginChunk, endChunk, renderer);
    }

    virtual void updateGeometry(Renderer&) override
    {
        auto resourceManager = m_sceneObject.getOwner().getLoop()->getResourceManager();
        ASSERT(resourceManager);

        for(Size s = 0; s < m_layerCount; s++)
        {
            m_atlasses[s] = resourceManager->getTexture(m_atlasNames[s]);
            ASSERT(m_atlasses[s]);
        }
    }

    SharedPtr<Texture> getTextureForLayer(EGE::Size layer)
    {
        ASSERT(layer < m_atlasses.size());
        return m_atlasses[layer];
    }

private:
    SharedPtr<TMap> m_tileMap;
    SharedPtrVector<Texture> m_atlasses;
    Vector<std::string> m_atlasNames;
    AtlasMapper m_tileMapper;
    bool m_useEnsure = false;
    EGE::Size m_layerCount = 1;
};

}
