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
#include <memory>
#include <SFML/Graphics.hpp>
#include <string>

namespace EGE
{

class ResourceManager
{
public:
    virtual bool reload() { return true; }
    void clear();
    bool isError();

    std::shared_ptr<Texture> getTexture(std::string name);
    std::shared_ptr<sf::Font> getFont(std::string name);
    std::shared_ptr<sf::Cursor> getCursor(std::string name);
    std::shared_ptr<sf::Cursor> getCursor(sf::Cursor::Type type);
    std::shared_ptr<sf::Shader> getShader(std::string name);
    std::shared_ptr<sf::Font> getDefaultFont();

protected:

    // Loads specified resources from file.
    // Returns nullptr when resource couldn't be loaded.
    std::shared_ptr<Texture> loadTextureFromFile(std::string fileName);
    std::shared_ptr<sf::Font> loadFontFromFile(std::string fileName);
    std::shared_ptr<sf::Cursor> loadCursorFromFile(std::string fileName);
    std::shared_ptr<sf::Shader> loadShaderFromFile(std::string fileName, sf::Shader::Type type);
    std::shared_ptr<sf::Shader> loadShaderFromFile(std::string name, std::string vertexShader, std::string fragmentShader);
    std::shared_ptr<sf::Shader> loadShaderFromFile(std::string name, std::string vertexShader, std::string geometryShader, std::string fragmentShader);

    // Adds preloaded resources to ResourceManager.
    // Useful when you want to add your options to resource before adding to RM.
    // If you specify nullptr as resource, it will be lazy-loaded from file on
    // first use. (it doesn't apply to shaders)
    void addTexture(std::string name, std::shared_ptr<Texture> texture = nullptr);
    void addFont(std::string name, std::shared_ptr<sf::Font> font = nullptr);
    void addCursor(std::string name, std::shared_ptr<sf::Cursor> cursor = nullptr);
    void addShader(std::string name, std::shared_ptr<sf::Shader> shader = nullptr);

    // TODO
    std::shared_ptr<sf::Font> loadSystemFont(std::string) { ASSERT(false); return nullptr; }
    std::shared_ptr<sf::Cursor> loadSystemCursor(sf::Cursor::Type);

    void setUnknownTexture(std::shared_ptr<Texture> texture = nullptr);

    // Sets the resource path (absolute or relative to current working directory)
    bool setResourcePath(std::string path);

    // NOTE: the font will be automatically added to lazy-load if not added.
    bool setDefaultFont(std::string name);
private:
    std::shared_ptr<Texture> m_unknownTexture;

    SharedPtrStringMap<Texture> m_loadedTextures;
    SharedPtrStringMap<sf::Font> m_loadedFonts;
    SharedPtrStringMap<sf::Cursor> m_loadedCursors;
    SharedPtrStringMap<sf::Shader> m_loadedShaders;

    bool m_error = false;
    bool m_systemCursorError = false;
    std::string m_resourcePath = "res";
protected:
    std::string m_defaultFont;
};

}
