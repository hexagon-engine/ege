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

#include "LayoutElement.h"

#include <algorithm>
#include <ege/debug/Logger.h>
#include <typeinfo>

namespace EGE
{

Vector<LayoutElement::_OutputDimensions> LayoutElement::calculateMainDimension(LayoutElement::_InputDimensions& thisDimensions, Vector<LayoutElement::_InputDimensions>& dimensions)
{
    if(dimensions.empty())
    {
        log(LogLevel::Debug) << "No children to layout!";
        return {};
    }

    // This should not fail since we do check in calculate().
    ASSERT(thisDimensions.size.isFixedUnit());

    double usedSpace = 0;
    size_t usedElements = 0;

    Vector<_OutputDimensions> output;

    // Known size & position
    // FIXME: Avoid copying
    for(_InputDimensions& element: dimensions)
    {
        // Convert % to px
        if(element.position.unit() == EGE_LAYOUT_PERCENT)
        {
            element.position.setUnit(EGE_LAYOUT_PIXELS);
            element.position.setValue(element.position.value() * thisDimensions.size.value() / 100.0);
        }

        if(element.size.unit() == EGE_LAYOUT_PERCENT)
        {
            element.size.setUnit(EGE_LAYOUT_PIXELS);
            element.size.setValue(element.size.value() * thisDimensions.size.value() / 100.0);
        }

        if(element.padding.unit() == EGE_LAYOUT_PERCENT)
        {
            element.padding.setUnit(EGE_LAYOUT_PIXELS);
            element.padding.setValue(element.padding.value() * element.size.value() / 100.0);
        }

        // Add elements with known size to layout
        if(element.position.unit() == EGE_LAYOUT_PIXELS && element.size.unit() == EGE_LAYOUT_PIXELS)
        {
            // Do nothing. Everything it's done.
            _OutputDimensions object;
            object.position = element.position.value();
            object.size = element.size.value();
            object.padding = element.padding.value();
            output.push_back(object);
            log(LogLevel::Debug) << "Add immediately!!";
        }
        else if(element.size.unit() != EGE_LAYOUT_FILL)
        {
            log(LogLevel::Debug) << "Count";
            // Do nothing but count to used elements
        }
        else
        {
            if(element.position.unit() == EGE_LAYOUT_PIXELS) // Known position, but unknown size !!!
            {
                // TODO
                element.position.setUnit(EGE_LAYOUT_FILL); // Ignore for now
            }
            log(LogLevel::Debug) << "Calculate size fill u=" << element.position.unit() << " su=" << element.size.unit();
            continue;
        }

        usedElements++;
        usedSpace += element.size.value();  // total space
    }

    double remainingSpace = thisDimensions.size.value() - usedSpace - thisDimensions.padding.value() * 2;
    double partSize = remainingSpace / (dimensions.size() - usedElements);

    /* Automatic resizing (if applicable) */
    if(thisDimensions.size.unit() == EGE_LAYOUT_AUTO)
    {
        log(LogLevel::Debug) << "Automatic resizing";
        thisDimensions.size.setUnit(EGE_LAYOUT_PIXELS);
        thisDimensions.size.setValue(usedSpace + thisDimensions.padding.value() * 2);
    }

    /* align=left */
    _InputDimensions last;
    last.position = LayoutSizeD("0px");
    last.size = LayoutSizeD("0px");

    for(_InputDimensions& element: dimensions)
    {
        if(element.align == LayoutAlign::Left)
        {
            log(LogLevel::Debug) << "align=left u=" << element.position.unit() << " su=" << element.size.unit();
            if(element.position.unit() == EGE_LAYOUT_FILL)
            {
                /* Set position relative to last part */
                element.position.setUnit(EGE_LAYOUT_PIXELS);
                element.position.setValue(last.position.value() + last.size.value());

                if(element.size.unit() == EGE_LAYOUT_FILL)
                {
                    /* If size is unknown, divide the remaining space into equal parts */
                    log(LogLevel::Debug) << "Divide L";
                    element.size.setUnit(EGE_LAYOUT_PIXELS);
                    element.size.setValue(partSize);
                }

                _OutputDimensions object;
                object.position = element.position.value() + thisDimensions.padding.value();
                object.size = element.size.value();
                object.padding = element.padding.value();
                output.push_back(object);
                log(LogLevel::Debug) << "Add align=left";

                last = element;
            }
        }
    }

    /* align=right */
    _InputDimensions next;
    next.position = LayoutSizeD("0px");
    next.size = LayoutSizeD(thisDimensions.size.value(), EGE_LAYOUT_PIXELS);

    for(long long s = dimensions.size() - 1; s >= 0; s--)
    {
        _InputDimensions& element = dimensions[s];
        if(element.align == LayoutAlign::Right)
        {
            log(LogLevel::Debug) << "align=right u=" << element.position.unit() << " su=" << element.size.unit();
            if(element.position.unit() == EGE_LAYOUT_FILL)
            {
                /* Set position relative to last part */
                element.position.setUnit(EGE_LAYOUT_PIXELS);
                element.position.setValue(next.position.value() - element.size.value());

                if(element.size.unit() == EGE_LAYOUT_FILL)
                {
                    /* If size is unknown, divide the remaining space into equal parts */
                    log(LogLevel::Debug) << "Divide R";
                    element.size.setUnit(EGE_LAYOUT_PIXELS);
                    element.size.setValue(partSize);
                }

                _OutputDimensions object;
                object.position = element.position.value() + thisDimensions.padding.value();
                object.size = element.size.value();
                object.padding = element.padding.value();
                output.push_back(object);
                log(LogLevel::Debug) << "Add align=right";

                // TODO: this should be outside this "if"?
                next = element;
            }
        }
    }

    return output;
}

Vector<LayoutElement::_OutputDimensions> LayoutElement::calculateOtherDimension(LayoutElement::_InputDimensions& thisDimensions, Vector<LayoutElement::_InputDimensions>& dimensions)
{
    auto calcPadding = [](double elSize, LayoutSizeD padding)->double
    {
        return padding.unit() == EGE_LAYOUT_PIXELS ? padding.value() : (padding.unit() == EGE_LAYOUT_PERCENT ? elSize * padding.value() : 0);
    };

    Vector<_OutputDimensions> output;

    // FIXME: Avoid copying
    for(_InputDimensions& element: dimensions)
    {
        if(element.size.unit() == EGE_LAYOUT_FILL)
            element.size.setValue(thisDimensions.size.value() - calcPadding(element.size.value(), thisDimensions.padding.value()) * 2);
        else if(element.size.unit() == EGE_LAYOUT_PERCENT)
            element.size.setValue(thisDimensions.size.value() * element.size.value() / 100 - calcPadding(element.size.value(), thisDimensions.padding.value()) * 2);

        if(element.position.unit() == EGE_LAYOUT_FILL)
            element.position = LayoutSizeD("0px");
        else if(element.position.unit() == EGE_LAYOUT_PERCENT)
            element.position.setValue(thisDimensions.position.value() * element.position.value() / 100);

        _OutputDimensions object;
        object.position = element.position.value() + thisDimensions.padding.value();
        object.size = element.size.value();
        object.padding = element.padding.value();
        output.push_back(object);
    }

    return output;
}

// returns elements with calculated layout (every Size in px-s)
// we assume that parent has the layout already calculated
// all coordinates are parent-relative
void LayoutElement::calculateLayout()
{
    if(!m_needRecalc)
    {
        log(LogLevel::Debug) << "LayoutElement::calculateLayout(): " << m_id << ": no recalc needed!";
        return;
    }

    /*std::cout << "Children: " << std::endl;
    for(auto& el: children)
    {
        std::cout << "   " << el->id << ": " << el->position.x.value() << el->position.x.unit() << ", " << el->position.y.value() << el->position.y.unit() << " @ "
                    << el->size.x.value() << el->size.x.unit() << ", " << el->size.y.value() << el->size.y.unit() << std::endl;
    }*/

    // Prepare "root" coordinates, assuming they are
    // set to fixed (px or A) value by user.
    if(!m_layout.original)
    {
        ASSERT_WITH_MESSAGE(m_size.x.isFixedUnit() && m_size.y.isFixedUnit(), "Root layout element must have fixed-unit (px or A) size!");
        // For position / padding, the default value is 0.

        m_layout.position = { m_position.x.value(), m_position.y.value() };
        m_layout.size = { m_size.x.value(), m_size.y.value() };
        m_layout.padding = { m_padding.x.value(), m_padding.y.value() };
        if(m_size.x.unit() == EGE_LAYOUT_AUTO) m_layout.autoSizingX = true;
        if(m_size.y.unit() == EGE_LAYOUT_AUTO) m_layout.autoSizingY = true;
    }

    // Prepare coords
    Vector<_InputDimensions> dimensionsX;
    Vector<_InputDimensions> dimensionsY;
    _InputDimensions thisDimensionsX;
    _InputDimensions thisDimensionsY;

    // FIXME: Avoid copying
    for(auto& child : m_children)
    {
        _InputDimensions objectX;
        objectX.position = child->m_position.x;
        objectX.size = child->m_size.x;
        objectX.padding = child->m_padding.x;
        dimensionsX.push_back(objectX);

        _InputDimensions objectY;
        objectY.position = child->m_position.y;
        objectY.size = child->m_size.y;
        objectY.padding = child->m_padding.y;
        dimensionsY.push_back(objectY);
    }

    thisDimensionsX.position = m_layout.position.x;
    thisDimensionsX.size = { m_layout.size.x, m_layout.autoSizingX ? EGE_LAYOUT_AUTO : EGE_LAYOUT_PIXELS };
    thisDimensionsX.padding = m_layout.padding.x;

    thisDimensionsY.position = m_layout.position.y;
    thisDimensionsY.size = { m_layout.size.y, m_layout.autoSizingY ? EGE_LAYOUT_AUTO : EGE_LAYOUT_PIXELS };
    thisDimensionsY.padding = m_layout.padding.y;

    log() << "Raw: id(" << m_id << ") pos(" << m_position.x << "," << m_position.y << ") size(" << m_size.x << "," << m_size.y << ") padding(" << m_padding.x << "," << m_padding.y << ")";
    log() << "Layout: pos(" << m_layout.position.x << "," << m_layout.position.y << ") size(" << m_layout.size.x << "," << m_layout.size.y << ") padding(" << m_layout.padding.x << "," << m_layout.padding.y << ")";

    // Calculate 'this' layout
    Vector<_OutputDimensions> layoutM, layoutO;

    switch(layoutDirection)
    {
        case Direction::Horizontal:
        {
            log(LogLevel::Debug) << "Horizontal layouting";
            layoutM = calculateMainDimension(thisDimensionsX, dimensionsX);
            layoutO = calculateOtherDimension(thisDimensionsY, dimensionsY);

        } break;
        case Direction::Vertical:
        {
            log(LogLevel::Debug) << "Vertical layouting";
            layoutM = calculateMainDimension(thisDimensionsY, dimensionsY);
            layoutO = calculateOtherDimension(thisDimensionsX, dimensionsX);

        } break;
    }

    ASSERT(layoutM.size() == layoutO.size());

    // FIXME: Avoid copying
    // Calculate each child's layout
    for(Size s = 0; s < layoutM.size(); s++)
    {
        // Add calculated coords to layout
        _LayoutElementCalculated calc;
        calc.original = m_children[s];

        auto lms = layoutM[s]; auto los = layoutO[s];
        switch(layoutDirection)
        {
            case Direction::Horizontal:
            {
                calc.position = Vec2d(lms.position, los.position);
                calc.size = Vec2d(lms.size, los.size);
                calc.padding = Vec2d(lms.padding, los.padding);
            } break;
            case Direction::Vertical:
            {
                calc.position = Vec2d(los.position, lms.position);
                calc.size = Vec2d(los.size, lms.size);
                calc.padding = Vec2d(los.padding, lms.padding);
            } break;
        }

        LayoutElement* child = calc.original;

        // Ensure that parent is properly set
        child->m_parent = this;

        // Calculate child's layout.
        child->m_layout = calc;
        child->calculateLayout();
    }
    m_needRecalc = false;
}

void LayoutElement::removeObject(LayoutElement* el)
{
    m_children.erase(std::find_if(m_children.begin(), m_children.end(), [el](LayoutElement* _el) {
        if(_el == el)
            return true;
        return false;
    }));
}

void LayoutElement::setRecalcNeeded()
{
    m_needRecalc = true;
    for(auto element: m_children)
        element->setRecalcNeeded();
}

}

