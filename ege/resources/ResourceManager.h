/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>
#include <ege/profiler/Profiler.h>
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

    std::shared_ptr<sf::Texture> getTexture(std::string name);
    std::shared_ptr<sf::Font> getFont(std::string name);
    std::shared_ptr<sf::Cursor> getCursor(std::string name);
    std::shared_ptr<sf::Cursor> getCursor(sf::Cursor::Type type);
    std::shared_ptr<sf::Shader> getShader(std::string name);
    std::shared_ptr<sf::Font> getDefaultFont();

protected:

    // Loads specified resources from file.
    // Returns nullptr when resource couldn't be loaded.
    std::shared_ptr<sf::Texture> loadTextureFromFile(std::string fileName);
    std::shared_ptr<sf::Font> loadFontFromFile(std::string fileName);
    std::shared_ptr<sf::Cursor> loadCursorFromFile(std::string fileName);
    std::shared_ptr<sf::Shader> loadShaderFromFile(std::string fileName, sf::Shader::Type type);
    std::shared_ptr<sf::Shader> loadShaderFromFile(std::string name, std::string vertexShader, std::string fragmentShader);
    std::shared_ptr<sf::Shader> loadShaderFromFile(std::string name, std::string vertexShader, std::string geometryShader, std::string fragmentShader);

    // Adds preloaded resources to ResourceManager.
    // Useful when you want to add your options to resource before adding to RM.
    // If you specify nullptr as resource, it will be lazy-loaded from file on
    // first use. (it doesn't apply to shaders)
    void addTexture(std::string name, std::shared_ptr<sf::Texture> texture = nullptr);
    void addFont(std::string name, std::shared_ptr<sf::Font> font = nullptr);
    void addCursor(std::string name, std::shared_ptr<sf::Cursor> cursor = nullptr);
    void addShader(std::string name, std::shared_ptr<sf::Shader> shader = nullptr);

    // TODO
    std::shared_ptr<sf::Font> loadSystemFont(std::string) { ASSERT(false); return nullptr; }
    std::shared_ptr<sf::Cursor> loadSystemCursor(sf::Cursor::Type);

    void setUnknownTexture(std::shared_ptr<sf::Texture> texture = nullptr);

    // Sets the resource path (absolute or relative to current working directory)
    bool setResourcePath(std::string path);

    // NOTE: the font will be automatically added to lazy-load if not added.
    bool setDefaultFont(std::string name);
private:
    std::shared_ptr<sf::Texture> m_unknownTexture;

    std::map<std::string, std::shared_ptr<sf::Texture>> m_loadedTextures;
    std::map<std::string, std::shared_ptr<sf::Font>> m_loadedFonts;
    std::map<std::string, std::shared_ptr<sf::Cursor>> m_loadedCursors;
    std::map<std::string, std::shared_ptr<sf::Shader>> m_loadedShaders;

    bool m_error = false;
    bool m_systemCursorError = false;
    std::string m_resourcePath = "res";
protected:
    std::string m_defaultFont;
};

}
