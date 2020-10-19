/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "RadioButton.h"

#include <ege/gui/GUIGameLoop.h>

namespace EGE
{

void RadioButton::renderOnly(sf::RenderTarget& target, const RenderStates&)
{
    sf::CircleShape cs;
    cs.setFillColor(sf::Color(255, 255, 255));
    cs.setOutlineThickness(1.f);

    // background
    cs.setPosition(2.f, 2.f);
    cs.setOutlineColor(sf::Color(60, 60, 60));
    cs.setRadius(5.5f);
    target.draw(cs);

    // border
    cs.setRadius(6.f);
    cs.setPosition(1.f, 1.f);
    cs.setFillColor(sf::Color::Transparent);
    cs.setOutlineColor(sf::Color(173, 173, 173));
    target.draw(cs);

    cs.setRadius(6.5f);
    cs.setPosition(1.f, 1.f);
    cs.setOutlineColor(sf::Color(210, 210, 210));
    target.draw(cs);

    // border if clicked
    if(m_leftClicked)
    {
        cs.setRadius(4.5f);
        cs.setPosition(3.f, 3.f);
        cs.setOutlineColor(sf::Color(70, 70, 70));
        target.draw(cs);
    }

    // check
    if(isChecked())
    {
        cs.setFillColor(sf::Color(60, 60, 60));
        cs.setOutlineColor(sf::Color::Transparent);
        cs.setPosition(5.f, 5.f);
        cs.setRadius(2.5f);
        target.draw(cs);
    }

    // label (generate)
    auto font = m_parent->getLoop()->getResourceManager().lock()->getDefaultFont();
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

void RadioButton::onClick(EGE::Vec2d pos)
{
    CheckBox::onClick(pos);
    /*
    RadioGroup* group = dynamic_cast<RadioGroup*>(m_parent);
    if(group)
    {
        group->deselectAllBut(this);
    }
    */
}

}
