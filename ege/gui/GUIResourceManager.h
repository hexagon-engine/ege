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

#include <ege/resources/ResourceManager.h>
#include <ege/resources/SFMLTexture.h>
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

    void registerUnknownTexture(SharedPtr<SFMLTexture> texture = nullptr) { m_unknownTextureToLoad = texture; }
    void registerDefaultFont(std::string name) { m_defaultFontToLoad = name; }

private:
    std::queue<String> m_texturesToLoad;
    std::queue<String> m_fontsToLoad;
    std::queue<String> m_cursorsToLoad;
    std::queue<std::pair<String, Vector<String>>> m_shadersToLoad;
    SharedPtr<SFMLTexture> m_unknownTextureToLoad;
    std::string m_defaultFontToLoad;
    bool m_loaded = false;
};

}
