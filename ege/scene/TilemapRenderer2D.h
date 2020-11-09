/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "ObjectRenderer.h"
#include "Scene2D.h"

#include <ege/gfx/Renderer.h>
#include <ege/tilemap/TileMap2D.h>

namespace EGE
{

template<class TT, class SizeType>
class TilemapRenderer2D : public ObjectRenderer
{
public:
    TilemapRenderer2D(std::shared_ptr<Scene> scene, std::shared_ptr<TileMap2D<TT, SizeType>> tilemap)
    : ObjectRenderer(scene), m_tileMap(tilemap) {}

    void setAtlasTextureName(std::string name)
    {
        if(m_atlasName != name)
        {
            m_atlasName = name;
            setGeometryNeedUpdate();
        }
    }

    // Function EGE::Vec2d map(TileType* tile);
    // The function maps tile type (in tilemap) to atlas coords to render (in pixels).
    void setTileAtlasMapper(std::function<EGE::Vec2d(const TT&)> mapper) { m_tileMapper = mapper; }

    virtual void render(const SceneObject& object, sf::RenderTarget& target, const RenderStates& states) const
    {
        // TODO: tilemap render cache (it should go to updateGeometry)
        ASSERT(m_tileMapper);
        EGE::Vec2u tileSize = m_tileMap->getTileSize();

        ASSERT(tileSize.x != 0);
        ASSERT(tileSize.y != 0);
        Scene2D* scene = (Scene2D*)m_scene.get();
        ASSERT(scene);

        SceneObject2D& sceneObject = (SceneObject2D&)object;

        // TODO: allow setting tilemap bounds
        sf::Vector2f beginCoord = scene->mapScreenToScene(target, sf::Vector2i(0, 0));
        sf::Vector2f endCoord = scene->mapScreenToScene(target, sf::Vector2i(target.getSize()));
        sf::Vector2f objPos = sceneObject.getPosition();

        EGE::Vector2<long long> beginTile = {
            (long long)((beginCoord.x - objPos.x) / tileSize.x),
            (long long)((beginCoord.y - objPos.y) / tileSize.y - 1)
        };
        EGE::Vector2<long long> endTile = {
            (long long)((endCoord.x - objPos.x) / tileSize.x + 1),
            (long long)((endCoord.y - objPos.y) / tileSize.y)
        };

        // TODO: calculate and reserve required space
        // TODO: don't check tiles outside the bounds
        // TODO: allow checking bounds inside tilemap if it's applicabled
        // TODO: multiple layers (e.g for shadows)
        std::vector<EGE::Vertex> vertexes;
        EGE::Renderer renderer(target);

        for(long long x = beginTile.x; x <= endTile.x; x++)
        for(long long y = beginTile.y; y <= endTile.y; y++)
        {
            const TT* tile = m_tileMap->getTile({(SizeType)x, (SizeType)y});
            if(!tile) continue;
            EGE::Vec2d atlasCoords = m_tileMapper(*tile);

            EGE::Vertex vertex;
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
        // TODO: allow using states
        ASSERT(m_atlas);
        RenderStates newStates = states;
        newStates.sfStates().texture = m_atlas.get();
        renderer.setStates(newStates);
        renderer.renderPrimitives(vertexes, sf::Quads);
    }

    virtual void updateGeometry(SceneObject&)
    {
        auto resourceManager = m_scene->getLoop()->getResourceManager();
        ASSERT(!resourceManager.expired());
        m_atlas = resourceManager.lock()->getTexture(m_atlasName);
        ASSERT(m_atlas);
    }

private:
    std::shared_ptr<TileMap2D<TT, SizeType>> m_tileMap;
    std::shared_ptr<sf::Texture> m_atlas;
    std::string m_atlasName;
    std::function<Vec2d(const TT&)> m_tileMapper;
};

}
