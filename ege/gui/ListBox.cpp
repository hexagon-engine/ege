/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ListBox.h"

#include "GUIGameLoop.h"

#include <ege/debug/Logger.h>
#include <ege/gfx/Renderer.h>

namespace EGE
{

ListBox::ListBox(Widget* parent)
: CompoundWidget(parent)
{
    m_scrollbar = make<ScrollBar>(this);
    m_scrollbar->setType(ScrollBar::Type::Vertical);
    addWidget(m_scrollbar);
}

void ListBox::addEntry(sf::String str)
{
    m_entries.push_back(str);
    if(m_entries.size() > 1)
        m_scrollbar->setMaxValue(m_entries.size() - 1);
}

void ListBox::removeEntry(size_t index)
{
    ASSERT(index < m_entries.size());
    if(m_selection >= index)
    {
    }
    m_entries.erase(m_entries.begin() + index);
    if(m_entries.size() > 1)
        m_scrollbar->setMaxValue(m_entries.size() - 1);
}

void ListBox::onMouseButtonPress(sf::Event::MouseButtonEvent& event)
{
    // TODO
    CompoundWidget::onMouseButtonPress(event);
    log(LogLevel::Debug) << "ListBox::onMouseButtonPress " << event.x << "," << event.y;
}

void ListBox::onKeyPress(sf::Event::KeyEvent& event)
{
    CompoundWidget::onKeyPress(event);
    if(!m_entries.empty() && hasFocus())
    {
        if(event.code == sf::Keyboard::Up)
        {
            if(((int)m_selection) - 1 >= 0)
            {
                m_selection--;
                if(m_updateSelectionCallback)
                    m_updateSelectionCallback(m_selection);
                m_scrollbar->setValue(m_selection);
            }
        }
        else if(event.code == sf::Keyboard::Down)
        {
            if(m_selection + 1 < m_entries.size())
            {
                m_selection++;
                if(m_updateSelectionCallback)
                    m_updateSelectionCallback(m_selection);
                m_scrollbar->setValue(m_selection);
            }
        }
        else if(event.code == sf::Keyboard::Enter)
        {
            if(m_activateCallback)
                m_activateCallback(m_selection);
        }
    }
}

void ListBox::renderOnly(sf::RenderTarget& target, const RenderStates& states)
{
    CompoundWidget::renderOnly(target, states);

    Renderer renderer(target);

    // border
    renderer.renderTextBoxLike(0.0, 0.0, m_size.x, m_size.y);

    // entries
    size_t counter = 0;
    for(auto entry: m_entries)
    {
        auto font = getLoop()->getResourceManager().lock()->getDefaultFont();
        ASSERT(font);
        if(counter == m_selection)
            renderer.renderRectangle(3.0, counter * 15.0 + 3.0, m_size.x - 20.0, 15.0, sf::Color(128, 128, 128));
        renderer.renderText(3.0, counter * 15.0 + 3.0, *font, entry, 12);
        counter++;
    }
}

void ListBox::setSize(EGE::Vec2d size)
{
    CompoundWidget::setSize(size);
    m_scrollbar->setLength(size.y);
    m_scrollbar->setPosition(EGE::Vec2d(size.x - 20.0, 0.0));
}

}
