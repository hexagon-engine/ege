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

#include "Text.h"

#include <ege/debug/Logger.h>

#define TEXT_DEBUG 1

namespace EGE
{

Vec2d Text::getSize()
{
    // TODO: calculate size if it's needed
    return m_size;
}

void Text::render(Renderer& renderer) const
{
    // TODO: word wrap
    // TODO: syntax highlighting
    // TODO: align to view to disable antialiasing

    ege_log.debug() << "Text::render()";
    sf::String utf32String(m_text);
    Vec2d currentPos = settings.position;
    Vec2d startPos = currentPos;
    auto& texture = m_font.getTexture(settings.fontSize);
    int lineSpacing = m_font.getLineSpacing(settings.fontSize);

    int minTop = 0;
    for(size_t s = 0; s < utf32String.getSize(); s++)
    {
        Uint32 code = utf32String[s];
        sf::Glyph glyph = m_font.getGlyph(code, settings.fontSize, settings.bold, 0);
        if(minTop > glyph.bounds.top)
            minTop = glyph.bounds.top;
    }

    for(size_t s = 0; s < utf32String.getSize(); s++)
    {
        Uint32 code = utf32String[s];
        sf::Glyph glyph = m_font.getGlyph(code, settings.fontSize, settings.bold, 0);
        // TODO: handle spaces and tabs
        ege_log.debug() << glyph.bounds.left << "," << glyph.bounds.top << "x" << glyph.bounds.width << "," << glyph.bounds.height;
        renderer.renderTexturedRectangle(currentPos.x + glyph.bounds.left, currentPos.y + glyph.bounds.top - minTop, glyph.textureRect.width, glyph.textureRect.height, texture, glyph.textureRect);
        
        if constexpr(TEXT_DEBUG)
            renderer.renderRectangle(currentPos.x + glyph.bounds.left, currentPos.y + glyph.bounds.top - minTop, glyph.textureRect.width, glyph.textureRect.height, Colors::transparent, Colors::magenta);
        
        float kerning = s == utf32String.getSize() - 1 ? 0 : m_font.getKerning(code, utf32String[s + 1], settings.fontSize);
        currentPos.x += glyph.advance + kerning;
    }
    if constexpr(TEXT_DEBUG)
        renderer.renderRectangle(startPos.x, startPos.y, currentPos.x - startPos.x, currentPos.y - startPos.y, Colors::transparent, Colors::magenta);
}

}

