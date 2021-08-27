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

#include "Texture.h"

#include <ege/main/Config.h>
#include <ege/debug/Profiler.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/Types.h>
#include <memory>
#include <SFML/Graphics.hpp>
#include <string>

namespace EGE
{

class ResourceManager
{
public:
    virtual ~ResourceManager() = default;
    virtual bool reload() { return true; }
    void clear();
    bool isError();

    SharedPtr<Texture> getTexture(String name);
    SharedPtr<sf::Font> getFont(String name);
    SharedPtr<sf::Cursor> getCursor(String name);
    SharedPtr<sf::Cursor> getCursor(sf::Cursor::Type type);
    SharedPtr<sf::Shader> getShader(String name);
    SharedPtr<sf::Font> getDefaultFont();

protected:

    // Loads specified resources from file.
    // Returns nullptr when resource couldn't be loaded.
    SharedPtr<Texture> loadTextureFromFile(String fileName);
    SharedPtr<sf::Font> loadFontFromFile(String fileName);
    SharedPtr<sf::Cursor> loadCursorFromFile(String fileName);
    SharedPtr<sf::Shader> loadShaderFromFile(String fileName, sf::Shader::Type type);
    SharedPtr<sf::Shader> loadShaderFromFile(String name, String vertexShader, String fragmentShader);
    SharedPtr<sf::Shader> loadShaderFromFile(String name, String vertexShader, String geometryShader, String fragmentShader);

    // Adds preloaded resources to ResourceManager.
    // Useful when you want to add your options to resource before adding to RM.
    // If you specify nullptr as resource, it will be lazy-loaded from file on
    // first use. (it doesn't apply to shaders)
    void addTexture(String name, SharedPtr<Texture> texture = nullptr);
    void addFont(String name, SharedPtr<sf::Font> font = nullptr);
    void addCursor(String name, SharedPtr<sf::Cursor> cursor = nullptr);
    void addShader(String name, SharedPtr<sf::Shader> shader = nullptr);

    // TODO
    SharedPtr<sf::Font> loadSystemFont(String) { NOT_IMPLEMENTED("Loading system font"); }
    SharedPtr<sf::Cursor> loadSystemCursor(sf::Cursor::Type);

    void setUnknownTexture(SharedPtr<Texture> texture = nullptr);

    // NOTE: the font will be automatically added to lazy-load if not added.
    bool setDefaultFont(String name);
private:
    SharedPtr<Texture> m_unknownTexture;

    SharedPtrStringMap<Texture> m_loadedTextures;
    SharedPtrStringMap<sf::Font> m_loadedFonts;
    SharedPtrStringMap<sf::Cursor> m_loadedCursors;
    SharedPtrStringMap<sf::Shader> m_loadedShaders;

    bool m_error = false;
    bool m_systemCursorError = false;
protected:
    String m_defaultFont;
};

}
