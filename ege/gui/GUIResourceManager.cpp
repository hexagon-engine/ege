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

#include "GUIResourceManager.h"

#include <ege/debug/Logger.h>

namespace EGE
{

bool GUIResourceManager::reload()
{
    if(m_loaded)
        return true;
    m_loaded = true;
    ege_log.info() << "GUIResourceManager is loading "
        << m_texturesToLoad.size() << " textures, "
        << m_fontsToLoad.size() << " fonts, "
        << m_cursorsToLoad.size() << " cursors and "
        << m_shadersToLoad.size() << " shaders.";

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
            ege_log.error() << "Invalid argument for shader loader (should be {Vertex, Geometry, Fragment} or {Vertex, Fragment})";
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
