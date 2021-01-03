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

#pragma once

#include "LayoutSize.h"

#include <ege/util/Vector.h>

#include <SFML/System/NonCopyable.hpp>

namespace EGE
{

enum class LayoutAlign
{
    Left,
    Right
};

class LayoutElement : public sf::NonCopyable
{
public:

    enum class Direction
    {
        Horizontal,
        Vertical
    };

    LayoutElement(LayoutElement* parent, String id = "???")
    : m_parent(parent), m_id(id) { if(m_parent) m_parent->addObject(this); }

    struct AlignVec2
    {
        LayoutAlign x = LayoutAlign::Left;
        LayoutAlign y = LayoutAlign::Left;
    };

    AlignVec2 align;
    Direction layoutDirection = Direction::Horizontal;

    struct _InputDimensions
    {
        LayoutSizeD position;
        LayoutSizeD size;
        LayoutSizeD padding;
        LayoutAlign align = LayoutAlign::Left;
    };

    struct _InputDimensionsVec2
    {
        _InputDimensions x, y;
        bool isRoot = true;
    };

    // %calculated - dimensions from parent (calculated in px) or manually
    // set by layout user (in px or A, isRoot = true - it's default)
    virtual void calculateLayout();

    // Get position as visible on screen, relative to parent, with padding
    // applied.
    /*Vec2d getPosition() const
    {
        return {(m_parent ? m_parent->m_layout.padding.x : 0) + m_layout.position.x,
                (m_parent ? m_parent->m_layout.padding.y : 0) + m_layout.position.y};
    }*/
    Vec2d getPosition() const
    {
        return {m_layout.position.x, m_layout.position.y};
    }

    Vec2d getAbsolutePosition() const { return m_parent ? m_parent->getAbsolutePosition() + getPosition() : getPosition(); }

    // Get size as visible on screen (after layouting etc., padding applied)
    //Vec2d getSize() const { return m_layout.size - m_layout.padding * 2.0; }
    Vec2d getSize() const { return m_layout.size; }
    Vec2d getPadding() const { return m_layout.padding; }

    LVec2d getRawPosition() const { return m_position; }
    LVec2d getRawSize() const { return m_size; }
    LVec2d getRawPadding() const { return m_padding; }

    virtual void setPosition(LVec2d position) { m_position = position; setRecalcNeeded(); }
    virtual void setSize(LVec2d size) { m_size = size; setRecalcNeeded(); }
    virtual void setPadding(LVec2d size) { m_padding = size; setRecalcNeeded(); }

    LayoutElement* getParent() const { return m_parent; }

    String getId() const { return m_id; }

protected:
    // NOTE: The user must keep "real" (strong) pointers to elements!
    void addObject(LayoutElement* el)
    {
        el->m_parent = this;
        m_children.push_back(el);
        setRecalcNeeded();
    }

    void removeObject(LayoutElement* el);

    Vector<LayoutElement*> m_children;

private:
    void setRecalcNeeded();

    struct _OutputDimensions
    {
        double position;
        double size;
        double padding;
    };

    struct _LayoutElementCalculated
    {
        LayoutElement* original;
        Vec2d position;
        Vec2d size;
        Vec2d padding;
        bool autoSizingX = false;
        bool autoSizingY = false;

        _LayoutElementCalculated(LayoutElement* _original = nullptr)
        : original(_original) {}
    };

    static Vector<_OutputDimensions> calculateMainDimension(_InputDimensions& thisDimensions, Vector<_InputDimensions>& dim);
    static Vector<_OutputDimensions> calculateOtherDimension(_InputDimensions& thisDimensions, Vector<_InputDimensions>& dim);

    LVec2d m_position;
    LVec2d m_size;
    LVec2d m_padding;

    // Calculated layout.
    _LayoutElementCalculated m_layout;
    LayoutElement* m_parent = nullptr;

    bool m_needRecalc = true;

    String m_id;
};

}
