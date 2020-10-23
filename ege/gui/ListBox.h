/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "CompoundWidget.h"
#include "ScrollBar.h"

#include <ege/gfx/RenderStates.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class ListBox : public CompoundWidget
{
public:
    ListBox(Widget* parent);

    void addEntry(sf::String str);
    void removeEntry(size_t index);
    sf::String getEntry(size_t index) { ASSERT(index < m_entries.size()); return m_entries[index]; }
    size_t getSelection() { return m_selection; }
    sf::String getEntryAtSelection() { return getEntry(getSelection()); }
    void setSelection(size_t sel) { ASSERT(sel < m_entries.size()); m_selection = sel; }

    void setUpdateSelectionCallback(std::function<void(size_t)> func) { m_updateSelectionCallback = func; }
    void setActivateCallback(std::function<void(size_t)> func) { m_activateCallback = func; }

    virtual void onMouseButtonPress(sf::Event::MouseButtonEvent& event);
    virtual void onKeyPress(sf::Event::KeyEvent& event);
    virtual void renderOnly(sf::RenderTarget& target, const RenderStates& states = {}) override;

    virtual void setSize(EGE::Vec2d size);

private:
    size_t m_selection = 0;
    std::vector<sf::String> m_entries;
    std::shared_ptr<ScrollBar> m_scrollbar = nullptr;
    std::function<void(size_t)> m_updateSelectionCallback;
    std::function<void(size_t)> m_activateCallback;
};

}
