/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "CheckBox.h"

#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void CheckBox::renderOnly(Renderer& renderer)
{
    sf::RenderTarget& target = renderer.getTarget();

    sf::RectangleShape rs;
    rs.setFillColor(sf::Color(255, 255, 255));
    rs.setOutlineThickness(1.f);

    // background
    rs.setPosition(2.f, 2.f);
    rs.setOutlineColor(sf::Color(60, 60, 60));
    rs.setSize(sf::Vector2f(11.f, 11.f));
    target.draw(rs);

    // border
    rs.setSize(sf::Vector2f(12.f, 12.f));
    rs.setPosition(1.f, 1.f);
    rs.setFillColor(sf::Color::Transparent);
    rs.setOutlineColor(sf::Color(173, 173, 173));
    target.draw(rs);

    rs.setSize(sf::Vector2f(13.f, 13.f));
    rs.setPosition(1.f, 1.f);
    rs.setOutlineColor(sf::Color(210, 210, 210));
    target.draw(rs);

    // border if clicked
    if(m_leftClicked)
    {
        rs.setSize(sf::Vector2f(9.f, 9.f));
        rs.setPosition(3.f, 3.f);
        rs.setOutlineColor(sf::Color(70, 70, 70));
        target.draw(rs);
    }

    // check
    if(m_checked)
    {
        rs.setFillColor(sf::Color(60, 60, 60));
        rs.setOutlineColor(sf::Color::Transparent);
        rs.setPosition(5.f, 5.f);
        rs.setSize(sf::Vector2f(5.f, 5.f));
        target.draw(rs);
    }

    // label (generate)
    auto font = m_parent->getLoop().getResourceManager().lock()->getDefaultFont();
    ASSERT(font);
    sf::Text text(getLabel(), *font, 12);

    if(m_geometryUpdate)
    {
        m_geometryUpdate = false;
        m_size = EGE::Vec2d(text.getLocalBounds().width + 25.f, 25.f);
    }

    // label
    text.setPosition(20.f, 0.f);
    text.setFillColor(sf::Color::Black);
    target.draw(text);
}

void CheckBox::onClick(EGE::Vec2d position)
{
    Button::onClick(position);
    m_checked = !m_checked;
}

}
