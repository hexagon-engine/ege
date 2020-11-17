/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "ObjectRenderer.h"
#include "Scene2D.h"

#include <ege/gfx/Renderer.h>
#include <ege/tilemap/TileMap2D.h>
#include <ege/util.h>

namespace EGE
{

template<class TMap>
class TilemapRenderer2D : public ObjectRenderer
{
public:
    TilemapRenderer2D(std::shared_ptr<Scene> scene, std::shared_ptr<TMap> tilemap)
    : ObjectRenderer(scene), m_tileMap(tilemap) { setLayerCount(1); }

    void setAtlasTextureName(std::string name, Size layer = 0)
    {
        ASSERT(layer < m_layerCount);
        if(m_atlasNames[layer] != name)
        {
            m_atlasNames[layer] = name;
            setGeometryNeedUpdate();
        }
    }

    // Function EGE::Vec2d map(TileType* tile, EGE::Size layer);
    // The function maps tile type (in tilemap) to atlas coords to render (in pixels).
    void setTileAtlasMapper(std::function<Vec2d(const typename TMap::TileType&, Size)> mapper) { m_tileMapper = mapper; }

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

    virtual void renderLayer(Size layer, sf::Vector2f objPos, Vector2<MaxInt> beginChunk, Vector2<MaxInt> endChunk, sf::RenderTarget& target, const RenderStates& states) const
    {
        sf::VertexArray vertexes(sf::Quads);
        Renderer renderer(target);

        Vec2u tileSize = m_tileMap->getTileSize();
        Vec2s chunkSize = m_tileMap->getChunkSize();

        ASSERT(tileSize.x != 0);
        ASSERT(tileSize.y != 0);
        ASSERT(chunkSize.x != 0);
        ASSERT(chunkSize.y != 0);

        Size arraySize = (endChunk.x - beginChunk.x + 1) * (endChunk.y - beginChunk.y + 1) * chunkSize.x * chunkSize.y * 4;
        vertexes.resize(arraySize);
        Size index = 0;

        // TODO: calculate and reserve required space
        // TODO: don't check tiles outside the bounds
        // TODO: allow checking bounds inside tilemap if it's applicable
        for(MaxInt cx = beginChunk.x; cx <= endChunk.x; cx++)
        for(MaxInt cy = beginChunk.y; cy <= endChunk.y; cy++)
        {
            const typename TMap::ChunkType* chunk = (m_useEnsure
                                                            ? &m_tileMap->ensureChunk({(typename TMap::SizeType)cx, (typename TMap::SizeType)cy})
                                                            : m_tileMap->getChunk({(typename TMap::SizeType)cx, (typename TMap::SizeType)cy})
                                                        );
            if(!chunk)
                continue;

            for(Size x = 0; x < chunkSize.x; x++)
            for(Size y = 0; y < chunkSize.y; y++)
            {
                const typename TMap::TileType& tile = chunk->getTile({x, y});

                Vec2d atlasCoords = m_tileMapper(tile, layer);

                double vx = cx * (MaxInt)chunkSize.x + (MaxInt)x;
                double vy = cy * (MaxInt)chunkSize.y + (MaxInt)y;

                sf::Vertex vertex;
                vertex.color.r = 255;
                vertex.color.g = 255;
                vertex.color.b = 255;
                vertex.color.a = 255;

                vertex.position.x = vx * tileSize.x + objPos.x;
                vertex.position.y = vy * tileSize.y + objPos.y;
                vertex.texCoords.x = atlasCoords.x;
                vertex.texCoords.y = atlasCoords.y;
                vertexes[index++] = (vertex);

                vertex.position.x = (vx + 1) * tileSize.x + objPos.x;
                vertex.position.y = vy * tileSize.y + objPos.y;
                vertex.texCoords.x = atlasCoords.x + tileSize.x;
                vertex.texCoords.y = atlasCoords.y;
                vertexes[index++] = (vertex);

                vertex.position.x = (vx + 1) * tileSize.x + objPos.x;
                vertex.position.y = (vy + 1) * tileSize.y + objPos.y;
                vertex.texCoords.x = atlasCoords.x + tileSize.x;
                vertex.texCoords.y = atlasCoords.y + tileSize.y;
                vertexes[index++] = (vertex);

                vertex.position.x = vx * tileSize.x + objPos.x;
                vertex.position.y = (vy + 1) * tileSize.y + objPos.y;
                vertex.texCoords.x = atlasCoords.x;
                vertex.texCoords.y = atlasCoords.y + tileSize.y;
                vertexes[index++] = (vertex);
            }
        }

        // TODO: use triangles
        auto texture = m_atlasses[layer];
        ASSERT(texture);
        RenderStates newStates = states;
        newStates.sfStates().texture = texture.get();
        target.draw(vertexes, newStates.sfStates());
    }

    virtual void render(const SceneObject& object, sf::RenderTarget& target, const RenderStates& states) const
    {
        // TODO: tilemap render cache (it should go to updateGeometry)
        ASSERT(m_tileMapper);

        Vec2u tileSize = m_tileMap->getTileSize();
        Vec2s chunkSize = m_tileMap->getChunkSize();

        ASSERT(tileSize.x != 0);
        ASSERT(tileSize.y != 0);
        ASSERT(chunkSize.x != 0);
        ASSERT(chunkSize.y != 0);

        Scene2D* scene = (Scene2D*)m_scene.get();
        ASSERT(scene);

        SceneObject2D& sceneObject = (SceneObject2D&)object;

        // TODO: allow setting tilemap bounds
        sf::Vector2f beginCoord = scene->mapScreenToScene(target, sf::Vector2i(0, 0));
        sf::Vector2f endCoord = scene->mapScreenToScene(target, sf::Vector2i(target.getSize()));
        sf::Vector2f objPos = sceneObject.getPosition();

        Vector2<MaxInt> beginChunk = {
            (MaxInt)((beginCoord.x - objPos.x) / ((MaxInt)tileSize.x * chunkSize.x) - 1),
            (MaxInt)((beginCoord.y - objPos.y) / ((MaxInt)tileSize.y * chunkSize.y) - 1)
        };
        Vector2<MaxInt> endChunk = {
            (MaxInt)((endCoord.x - objPos.x) / ((MaxInt)tileSize.x * chunkSize.x) + 1),
            (MaxInt)((endCoord.y - objPos.y) / ((MaxInt)tileSize.y * chunkSize.y) + 1)
        };

        for(Size s = 0; s < m_layerCount; s++)
            renderLayer(s, objPos, beginChunk, endChunk, target, states);
    }

    virtual void updateGeometry(SceneObject&)
    {
        auto resourceManager = m_scene->getLoop()->getResourceManager();
        ASSERT(!resourceManager.expired());

        for(Size s = 0; s < m_layerCount; s++)
        {
            m_atlasses[s] = resourceManager.lock()->getTexture(m_atlasNames[s]);
            ASSERT(m_atlasses[s]);
        }
    }

private:
    std::shared_ptr<TMap> m_tileMap;
    SharedPtrVector<sf::Texture> m_atlasses;
    Vector<std::string> m_atlasNames;
    std::function<Vec2d(const typename TMap::TileType&, EGE::Size)> m_tileMapper;
    bool m_useEnsure = false;
    EGE::Size m_layerCount = 1;
};

}
