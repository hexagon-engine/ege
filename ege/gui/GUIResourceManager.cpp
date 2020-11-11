/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "GUIResourceManager.h"

#include <ege/debug/Logger.h>

namespace EGE
{

bool GUIResourceManager::reload()
{
    log(LogLevel::Info) << "GUIResourceManager is loading "
        << m_texturesToLoad.size() << " textures, "
        << m_fontsToLoad.size() << " fonts, "
        << m_cursorsToLoad.size() << " cursors and "
        << m_shadersToLoad.size() << " shaders.";

    if(!m_resourcePathToLoad.empty())
        setResourcePath(m_resourcePathToLoad);

    while(!m_texturesToLoad.empty())
    {
        if(!loadTextureFromFile(m_texturesToLoad.front()))
            return false;
        m_texturesToLoad.pop();
    }

    while(!m_fontsToLoad.empty())
    {
        if(!loadFontFromFile(m_fontsToLoad.front()))
            return false;
        m_fontsToLoad.pop();
    }

    while(!m_cursorsToLoad.empty())
    {
        if(!loadCursorFromFile(m_cursorsToLoad.front()))
            return false;
        m_cursorsToLoad.pop();
    }

    while(!m_shadersToLoad.empty())
    {
        auto& arr = m_shadersToLoad.front();
        if(arr.second.size() == 2)
        {
            if(!loadShaderFromFile(arr.first, arr.second[0], arr.second[1]))
                return false;
        }
        else if(arr.second.size() == 3)
        {
            if(!loadShaderFromFile(arr.first, arr.second[0], arr.second[1], arr.second[2]))
                return false;
        }
        else
        {
            err() << "Invalid argument for shader loader (should be {Vertex, Geometry, Fragment} or {Vertex, Fragment})";
        }

        m_shadersToLoad.pop();
    }

    if(m_unknownTextureToLoad)
        setUnknownTexture(m_unknownTextureToLoad);

    if(!m_defaultFontToLoad.empty())
        setDefaultFont(m_defaultFontToLoad);

    return true;
}

}
