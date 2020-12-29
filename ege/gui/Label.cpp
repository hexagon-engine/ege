/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Label.h"
#include "GUIGameLoop.h"

namespace EGE
{

void Label::setString(sf::String str)
{
    m_string = str;
    setGeometryNeedUpdate();
}

void Label::setTextAlign(Align align)
{
    m_align = align;
    setGeometryNeedUpdate();
}

void Label::setFontSize(int size)
{
    m_fontSize = size;
    setGeometryNeedUpdate();
}

void Label::setPosition(EGE::Vec2d position)
{
    setPositionInternal(position);
    m_textPosition = position;
    setGeometryNeedUpdate();
};

void Label::setPositionInternal(EGE::Vec2d position)
{
    Widget::setPosition(position);
}

void Label::updateGeometry(Renderer&)
{
    // FIXME: label is weirdly clipped (':('; '???')
    ASSERT(getLoop().getResourceManager());
    if(!m_font)
    {
        m_font = getLoop().getResourceManager()->getDefaultFont();
    }
    sf::Text text(m_string, *m_font, m_fontSize);
    sf::FloatRect bounds = text.getLocalBounds();
    bounds.height += 7.f * m_fontSize / 15.f; //SFML text bounds bug??
    bounds.width += 1.f * m_fontSize / 15.f;
    m_size = EGE::Vec2d(bounds.width, bounds.height);
    switch(m_align)
    {
        case Align::Left:
            text.setOrigin(0.f, 0.f);
            setPositionInternal(EGE::Vec2d(m_textPosition.x, m_textPosition.y));
            break;
        case Align::Center:
            text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
            setPositionInternal(EGE::Vec2d(m_textPosition.x - bounds.width / 2.f, m_textPosition.y - bounds.height / 2.f));
            break;
        case Align::Right:
            text.setOrigin(bounds.width, bounds.height);
            setPositionInternal(EGE::Vec2d(m_textPosition.x - bounds.width, m_textPosition.y - bounds.height));
            break;
    }
    text.setPosition(sf::Vector2f(m_textPosition.x, m_textPosition.y) - sf::Vector2f(getPosition().x, getPosition().y));
    text.setFillColor(m_color);
    m_text = text;
}

void Label::render(Renderer& renderer) const
{
    renderer.getTarget().draw(m_text);
}

}
