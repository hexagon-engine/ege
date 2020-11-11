/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/resources/ResourceManager.h>
#include <queue>

namespace EGE
{

class GUIResourceManager : public ResourceManager
{
public:
    void registerTexture(String name) { m_texturesToLoad.push(name); }
    void registerFont(String name) { m_fontsToLoad.push(name); }
    void registerCursor(String name) { m_cursorsToLoad.push(name); }

    // {Vertex, Geometry, Fragment} OR {Vertex, Fragment}
    void registerShader(String name, Vector<String> files) { m_shadersToLoad.push(std::make_pair(name, files)); }

    virtual bool reload();

    void registerUnknownTexture(std::shared_ptr<sf::Texture> texture = nullptr);
    bool registerResourcePath(std::string path);
    bool registerDefaultFont(std::string name);

private:
    std::queue<String> m_texturesToLoad;
    std::queue<String> m_fontsToLoad;
    std::queue<String> m_cursorsToLoad;
    std::queue<std::pair<String, Vector<String>>> m_shadersToLoad;
    std::shared_ptr<sf::Texture> m_unknownTextureToLoad;
    std::string m_resourcePathToLoad;
    std::string m_defaultFontToLoad;
};

}
