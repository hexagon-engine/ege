/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ege/main/Config.h>
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
    std::shared_ptr<sf::Font> getDefaultFont();

protected:

    // Loads specified resources from file.
    // Returns nullptr when texture couldn't be loaded.
    std::shared_ptr<sf::Texture> loadTextureFromFile(std::string fileName);
    std::shared_ptr<sf::Font> loadFontFromFile(std::string fileName);
    std::shared_ptr<sf::Cursor> loadCursorFromFile(std::string fileName);

    // Adds preloaded resources to ResourceManager.
    // Useful when you want to add your options to Texture/Font/Cursor before adding to RM.
    // If you specify nullptr as texture/font/cursor, it will be lazy-loaded
    // from file on first use.
    void addTexture(std::string name, std::shared_ptr<sf::Texture> texture = nullptr);
    void addFont(std::string name, std::shared_ptr<sf::Font> font = nullptr);
    void addCursor(std::string name, std::shared_ptr<sf::Cursor> cursor = nullptr);

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

    bool m_error = false;
    bool m_systemCursorError = false;
    std::string m_resourcePath = "res";
protected:
    std::string m_defaultFont;
};

}
