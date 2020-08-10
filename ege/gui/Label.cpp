/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Label.h"
#include "GUIGameLoop.h"

namespace EGE
{

void Label::setTextAlign(Align align)
{
    m_align = align;
    m_geometryChanged = true;
}

void Label::setFontSize(int size)
{
    m_fontSize = size;
    m_geometryChanged = true;
}

void Label::setTextPosition(sf::Vector2f position)
{
    m_textPosition = position;
    m_geometryChanged = true;
}

void Label::updateGeometry()
{
    if(m_geometryChanged)
    {
        if(!m_font)
        {
            m_font = getLoop()->getResourceManager().lock()->getDefaultFont();
        }
        sf::Text text(m_string, *m_font, m_fontSize);
        sf::FloatRect bounds = text.getLocalBounds();
        bounds.height += 5.f; //SFML text bounds bug??
        m_size = sf::Vector2f(bounds.width, bounds.height);
        switch(m_align)
        {
            case Align::Left:
                text.setOrigin(0.f, 0.f);
                setPosition(sf::Vector2f(m_textPosition.x, m_textPosition.y));
                break;
            case Align::Center:
                text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
                setPosition(sf::Vector2f(m_textPosition.x - bounds.width / 2.f, m_textPosition.y - bounds.height / 2.f));
                break;
            case Align::Right:
                text.setOrigin(bounds.width, bounds.height);
                setPosition(sf::Vector2f(m_textPosition.x - bounds.width, m_textPosition.y - bounds.height));
        }
        text.setPosition(m_textPosition - getPosition());
        m_text = text;
        m_geometryChanged = false;
    }
}

void Label::render(sf::RenderTarget& target)
{
    updateGeometry();
    Widget::render(target);
    m_text.setFillColor(sf::Color::Black);
    target.draw(m_text);
}

}
