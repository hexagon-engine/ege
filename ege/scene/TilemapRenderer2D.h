/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "ObjectRenderer.h"
#include "Scene2D.h"

#include <ege/gfx/Renderer.h>
#include <ege/tilemap/TileMap2D.h>
#include <ege/util/Types.h>

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

    virtual void renderLayer(Size layer, sf::Vector2f objPos, Vector2<MaxInt> beginTile, Vector2<MaxInt> endTile, sf::RenderTarget& target, const RenderStates& states) const
    {
        std::vector<Vertex> vertexes;
        Renderer renderer(target);

        Vec2u tileSize = m_tileMap->getTileSize();

        ASSERT(tileSize.x != 0);
        ASSERT(tileSize.y != 0);

        // TODO: calculate and reserve required space
        // TODO: don't check tiles outside the bounds
        // TODO: allow checking bounds inside tilemap if it's applicable
        for(MaxInt x = beginTile.x; x <= endTile.x; x++)
        for(MaxInt y = beginTile.y; y <= endTile.y; y++)
        {
            const typename TMap::TileType* tile = (m_useEnsure
                                                        ? &m_tileMap->ensureTile({(typename TMap::SizeType)x, (typename TMap::SizeType)y})
                                                        : m_tileMap->getTile({(typename TMap::SizeType)x, (typename TMap::SizeType)y})
                                                    );
            if(!tile) continue;
            Vec2d atlasCoords = m_tileMapper(*tile, layer);

            Vertex vertex;
            vertex.r = 127;
            vertex.g = 127;
            vertex.b = 127;

            vertex.x = x * tileSize.x + objPos.x;
            vertex.y = y * tileSize.y + objPos.y;
            vertex.texX = atlasCoords.x;
            vertex.texY = atlasCoords.y;
            vertexes.push_back(vertex);

            vertex.x = (x + 1) * tileSize.x + objPos.x;
            vertex.y = y * tileSize.y + objPos.y;
            vertex.texX = atlasCoords.x + tileSize.x;
            vertex.texY = atlasCoords.y;
            vertexes.push_back(vertex);

            vertex.x = (x + 1) * tileSize.x + objPos.x;
            vertex.y = (y + 1) * tileSize.y + objPos.y;
            vertex.texX = atlasCoords.x + tileSize.x;
            vertex.texY = atlasCoords.y + tileSize.y;
            vertexes.push_back(vertex);

            vertex.x = x * tileSize.x + objPos.x;
            vertex.y = (y + 1) * tileSize.y + objPos.y;
            vertex.texX = atlasCoords.x;
            vertex.texY = atlasCoords.y + tileSize.y;
            vertexes.push_back(vertex);

            // DEBUG
            //renderer.renderRectangle(vertex.x - tileSize.x, vertex.y - tileSize.y, tileSize.x, tileSize.y, sf::Color::Red, sf::Color::Green);
        }

        // TODO: use triangles
        auto texture = m_atlasses[layer];
        ASSERT(texture);
        RenderStates newStates = states;
        newStates.sfStates().texture = texture.get();
        renderer.setStates(newStates);
        renderer.renderPrimitives(vertexes, sf::Quads);
    }

    virtual void render(const SceneObject& object, sf::RenderTarget& target, const RenderStates& states) const
    {
        // TODO: tilemap render cache (it should go to updateGeometry)
        ASSERT(m_tileMapper);
        Vec2u tileSize = m_tileMap->getTileSize();

        ASSERT(tileSize.x != 0);
        ASSERT(tileSize.y != 0);
        Scene2D* scene = (Scene2D*)m_scene.get();
        ASSERT(scene);

        SceneObject2D& sceneObject = (SceneObject2D&)object;

        // TODO: allow setting tilemap bounds
        sf::Vector2f beginCoord = scene->mapScreenToScene(target, sf::Vector2i(0, 0));
        sf::Vector2f endCoord = scene->mapScreenToScene(target, sf::Vector2i(target.getSize()));
        sf::Vector2f objPos = sceneObject.getPosition();

        Vector2<MaxInt> beginTile = {
            (MaxInt)((beginCoord.x - objPos.x) / tileSize.x - 1),
            (MaxInt)((beginCoord.y - objPos.y) / tileSize.y - 1)
        };
        Vector2<MaxInt> endTile = {
            (MaxInt)((endCoord.x - objPos.x) / tileSize.x + 1),
            (MaxInt)((endCoord.y - objPos.y) / tileSize.y + 1)
        };

        // TODO: chunk rendering for chunk-capable tilemaps
        for(Size s = 0; s < m_layerCount; s++)
            renderLayer(s, objPos, beginTile, endTile, target, states);
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
