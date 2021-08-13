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

#include "GroupBox.h"

#include "GUIGameLoop.h"

namespace EGE
{

void GroupBox::render(Renderer& renderer) const
{
    sf::RenderTarget& target = renderer.getTarget();

    // Frame (generate)
    auto size = getSize();
    sf::RectangleShape rsFrame(sf::Vector2f(size.x, size.y) - sf::Vector2f(2.f, 2.f));
    rsFrame.setPosition(1.f, 1.f);
    rsFrame.setOutlineColor(sf::Color(164, 164, 164));
    rsFrame.setOutlineThickness(1.f);
    rsFrame.setFillColor(sf::Color::Transparent);

    // Frame 2 (generate)
    sf::RectangleShape rsFrame2(sf::Vector2f(size.x, size.y) - sf::Vector2f(4.f, 4.f));
    rsFrame2.setPosition(2.f, 2.f);
    rsFrame2.setOutlineColor(sf::Color(64, 64, 64));
    rsFrame2.setOutlineThickness(1.f);
    rsFrame2.setFillColor(sf::Color::Transparent);

    // Label (generate)
    auto font = getResourceManager()->getDefaultFont();
    ASSERT(font);
    sf::Text text(m_label, *font, 12);
    text.setFillColor(sf::Color(m_labelColor.r * 255, m_labelColor.g * 255, m_labelColor.b * 255, m_labelColor.a * 255));
    text.setOrigin(text.getLocalBounds().width / 2.f, 0.f);
    text.setPosition(size.x / 2.f, 0.f);

    // Label background (generate)
    sf::RectangleShape rsBg(sf::Vector2f(text.getLocalBounds().width + 10.f, text.getLocalBounds().height * 2.f + 6.f));
    rsBg.setOrigin(rsBg.getSize() / 2.f);
    auto bgColor = getWindow().getBackgroundColor();
    rsBg.setFillColor({ static_cast<sf::Uint8>(bgColor.r * 255),
                        static_cast<sf::Uint8>(bgColor.g * 255),
                        static_cast<sf::Uint8>(bgColor.b * 255),
                        static_cast<sf::Uint8>(bgColor.a * 255)});
    rsBg.setPosition(size.x / 2.f, 0.f);

    // Frame (draw)
    rsFrame.move(0.f, 6.0);
    rsFrame.setSize(sf::Vector2f(rsFrame.getSize().x, rsFrame.getSize().y - 6.0));
    target.draw(rsFrame, renderer.getStates().sfStates());

    rsFrame2.move(0.f, 6.0);
    rsFrame2.setSize(sf::Vector2f(rsFrame2.getSize().x, rsFrame2.getSize().y - 6.0));
    target.draw(rsFrame2, renderer.getStates().sfStates());

    // Label background (draw)
    target.draw(rsBg, renderer.getStates().sfStates());

    // Label (draw)
    target.draw(text, renderer.getStates().sfStates());

    Widget::render(renderer);
}

}
