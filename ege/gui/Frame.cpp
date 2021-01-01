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

#include "Frame.h"

#include "GUIGameLoop.h"

namespace EGE
{

void Frame::render(Renderer& renderer) const
{
    sf::RenderTarget& target = renderer.getTarget();

    // Frame (generate)
    sf::RectangleShape rsFrame(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(2.f, 2.f));
    rsFrame.setPosition(1.f, 1.f);
    rsFrame.setOutlineColor(sf::Color(164, 164, 164));
    rsFrame.setOutlineThickness(1.f);
    rsFrame.setFillColor(sf::Color::Transparent);

    // Frame 2 (generate)
    sf::RectangleShape rsFrame2(sf::Vector2f(m_size.x, m_size.y) - sf::Vector2f(4.f, 4.f));
    rsFrame2.setPosition(2.f, 2.f);
    rsFrame2.setOutlineColor(sf::Color(64, 64, 64));
    rsFrame2.setOutlineThickness(1.f);
    rsFrame2.setFillColor(sf::Color::Transparent);

    // Label (generate)
    auto font = m_parent->getLoop().getResourceManager()->getDefaultFont();
    ASSERT(font);
    sf::Text text(m_label, *font, 12);
    text.setFillColor(sf::Color::Black);
    text.setOrigin(text.getLocalBounds().width / 2.f, 0.f);
    text.setPosition(m_size.x / 2.f, 0.f);

    // Label background (generate)
    sf::RectangleShape rsBg(sf::Vector2f(text.getLocalBounds().width + 10.f, text.getLocalBounds().height * 2.f));
    rsBg.setOrigin(rsBg.getSize() / 2.f);
    rsBg.setFillColor(m_parent->getLoop().getBackgroundColor());
    rsBg.setPosition(m_size.x / 2.f, 0.f);

    // Frame (draw)
    rsFrame.move(0.f, rsBg.getSize().y / 4.f);
    rsFrame.setSize(sf::Vector2f(rsFrame.getSize().x, rsFrame.getSize().y - rsBg.getSize().y / 4.f));
    target.draw(rsFrame);

    rsFrame2.move(0.f, rsBg.getSize().y / 4.f);
    rsFrame2.setSize(sf::Vector2f(rsFrame2.getSize().x, rsFrame2.getSize().y - rsBg.getSize().y / 4.f));
    target.draw(rsFrame2);

    // Label background (draw)
    target.draw(rsBg);

    // Label (draw)
    target.draw(text);
}

}
