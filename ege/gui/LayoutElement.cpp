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
    ege_log.verbose() << "-- calculateMainDimension() --";
    if(dimensions.empty())
    {
        ege_log.debug() << "No children to layout!";
        return {};
    }

    // This should not fail since we do check in calculate().
    ASSERT(thisDimensions.size.isFixedUnit());

    double usedSpace = 0;
    size_t usedElements = 0;

    Vector<_OutputDimensions> output;
    output.resize(dimensions.size());

    // Known size & position
    // FIXME: Avoid copying
    for(size_t s = 0; s < dimensions.size(); s++)
    {
        auto& element = dimensions[s];
        ege_log.verbose() << "-- Setup --";
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
        if(element.position.unit() == EGE_LAYOUT_PIXELS && element.size.unit() == EGE_LAYOUT_PIXELS) // Known position & size
        {
            // Do nothing. Everything it's done.
            _OutputDimensions object;
            object.position = element.position.value();
            object.size = element.size.value();
            object.padding = element.padding.value();
            object.spacing = element.spacing;
            output[s] = object;
            ege_log.debug() <<  "Add immediately and count to usedElements (but not usedSize)";
            usedElements++;
            continue;
        }
        else if(element.size.unit() != EGE_LAYOUT_FILL) // Known size but unknown position
        {
            ege_log.debug() << "Count";
            // Do nothing but count to used elements
        }
        else
        {
            if(element.position.unit() == EGE_LAYOUT_PIXELS) // Known position, but unknown size !!!
            {
                // TODO
                element.position.setUnit(EGE_LAYOUT_FILL); // Ignore for now
            }
            ege_log.debug() << "Calculate size fill u=" << element.position.unit() << " su=" << element.size.unit();
            continue;
        }

        usedElements++;
        usedSpace += element.size.value();  // total space
    }

    size_t remainingElements = dimensions.size() - usedElements;
    double remainingSpace = thisDimensions.size.value() - usedSpace - thisDimensions.padding.value() * 2;
    double partSize = remainingSpace / remainingElements - thisDimensions.spacing;

    // Auto sizing!
    if(thisDimensions.size.unit() == EGE_LAYOUT_AUTO)
    {
        thisDimensions.size.setUnit(EGE_LAYOUT_PIXELS);
        thisDimensions.size.setValue(usedSpace + thisDimensions.padding.value() * 2);
    }

    /* align=left */
    _InputDimensions last;
    last.position = LayoutSizeD("0px");
    last.size = LayoutSizeD("0px");

    for(size_t s = 0; s < dimensions.size(); s++)
    {
        auto& element = dimensions[s];
        ege_log.verbose() << "-- Left-align CMD --";
        if(element.align == LayoutAlign::Left)
        {
            ege_log.debug() << "align=left u=" << element.position.unit() << " su=" << element.size.unit();
            if(element.position.unit() == EGE_LAYOUT_FILL)
            {
                /* Set position relative to last part */
                element.position.setUnit(EGE_LAYOUT_PIXELS);
                element.position.setValue(last.position.value() + last.size.value());

                if(element.size.unit() == EGE_LAYOUT_FILL)
                {
                    /* If size is unknown, divide the remaining space into equal parts */
                    ege_log.debug() << "Divide L";
                    element.size.setUnit(EGE_LAYOUT_PIXELS);
                    element.size.setValue(partSize);
                }

                _OutputDimensions object;
                object.position = element.position.value() + thisDimensions.padding.value();
                object.size = element.size.value();
                object.padding = element.padding.value();
                object.spacing = element.spacing;
                output[s] = object;
                ege_log.debug() << "Add align=left";

                element.position.setValue(element.position.value() + thisDimensions.spacing);
                last = element;
            }
        }
    }

    /* align=right */
    _InputDimensions next;
    next.position = LayoutSizeD(thisDimensions.size.value());
    next.size = LayoutSizeD(thisDimensions.size.value(), EGE_LAYOUT_PIXELS);

    for(size_t s = 0; s < dimensions.size(); s++)
    {
        auto& element = dimensions[s];
        ege_log.verbose() << "-- Right-align CMD --";

        if(element.align == LayoutAlign::Right)
        {
            ege_log.debug() << "align=right u=" << element.position.unit() << " su=" << element.size.unit();
            if(element.position.unit() == EGE_LAYOUT_FILL)
            {
                /* Set position relative to last part */
                element.position.setUnit(EGE_LAYOUT_PIXELS);
                element.position.setValue(next.position.value() - element.size.value());

                if(element.size.unit() == EGE_LAYOUT_FILL)
                {
                    /* If size is unknown, divide the remaining space into equal parts */
                    ege_log.debug() << "Divide R";
                    element.size.setUnit(EGE_LAYOUT_PIXELS);
                    element.size.setValue(partSize);
                }

                _OutputDimensions object;
                object.position = element.position.value() - thisDimensions.padding.value();
                object.size = element.size.value();
                object.padding = element.padding.value();
                object.spacing = element.spacing;
                output[s] = object;
                ege_log.debug() << "Add align=right";

                // TODO: this should be outside this "if"?
                element.position.setValue(element.position.value() - thisDimensions.spacing);
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
    // TODO: Automatic resizing
    if(!m_needRecalc)
    {
        ege_log.debug() << "LayoutElement::calculateLayout(): " << m_id << ": no recalc needed!";
        return;
    }

    // Prepare "root" coordinates, assuming they are
    // set to fixed (px or A) value by user.
    if(!m_layout.original)
    {
        ASSERT_WITH_MESSAGE(m_size.x.isFixedUnit() && m_size.y.isFixedUnit(), "Root layout element must have fixed-unit (px or A) size!");
        // For position / padding, the default value is 0.

        m_layout.position = { m_position.x.value(), m_position.y.value() };
        m_layout.size = { std::max(0.0, m_size.x.value()), std::max(0.0, m_size.y.value()) };
        m_layout.padding = { m_padding.x.value(), m_padding.y.value() };
        m_layout.spacing = m_spacing;
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
        objectX.align = child->align.x;
        objectX.spacing = child->m_spacing;
        dimensionsX.push_back(objectX);

        _InputDimensions objectY;
        objectY.position = child->m_position.y;
        objectY.size = child->m_size.y;
        objectY.padding = child->m_padding.y;
        objectY.align = child->align.y;
        objectY.spacing = child->m_spacing;
        dimensionsY.push_back(objectY);
    }

    thisDimensionsX.position = m_layout.position.x;
    thisDimensionsX.size = { m_layout.size.x, m_layout.autoSizingX ? EGE_LAYOUT_AUTO : EGE_LAYOUT_PIXELS };
    thisDimensionsX.padding = m_layout.padding.x;
    thisDimensionsX.align = align.x;
    thisDimensionsX.spacing = m_spacing;

    thisDimensionsY.position = m_layout.position.y;
    thisDimensionsY.size = { m_layout.size.y, m_layout.autoSizingY ? EGE_LAYOUT_AUTO : EGE_LAYOUT_PIXELS };
    thisDimensionsY.padding = m_layout.padding.y;
    thisDimensionsY.align = align.y;
    thisDimensionsY.spacing = m_spacing;

    ege_log.debug() << "Raw: id(" << m_id << ") pos(" << m_position.x << "," << m_position.y << ") size(" << m_size.x << "," << m_size.y << ") padding(" << m_padding.x << "," << m_padding.y << ")";
    ege_log.debug() << "Layout: pos(" << m_layout.position.x << "," << m_layout.position.y << ") size(" << m_layout.size.x << "," << m_layout.size.y << ") padding(" << m_layout.padding.x << "," << m_layout.padding.y << ")";

    // Calculate 'this' layout
    Vector<_OutputDimensions> layoutM, layoutO;

    switch(layoutDirection)
    {
        case Direction::Horizontal:
        {
            ege_log.debug() << "Horizontal layouting";
            layoutM = calculateMainDimension(thisDimensionsX, dimensionsX);
            layoutO = calculateOtherDimension(thisDimensionsY, dimensionsY);

        } break;
        case Direction::Vertical:
        {
            ege_log.debug() << "Vertical layouting";
            layoutM = calculateMainDimension(thisDimensionsY, dimensionsY);
            layoutO = calculateOtherDimension(thisDimensionsX, dimensionsX);

        } break;
    }

    // Auto-sizing
    if(m_layout.autoSizingX)
        m_layout.size.x = std::max(0.0, thisDimensionsX.size.value());
    if(m_layout.autoSizingY)
        m_layout.size.y = std::max(0.0, thisDimensionsY.size.value());

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
                calc.size = Vec2d(std::max(0.0, lms.size), std::max(0.0, los.size));
                calc.padding = Vec2d(lms.padding, los.padding);
            } break;
            case Direction::Vertical:
            {
                calc.position = Vec2d(los.position, lms.position);
                calc.size = Vec2d(std::max(0.0, los.size), std::max(0.0, lms.size));
                calc.padding = Vec2d(los.padding, lms.padding);
            } break;
        }

        LayoutElement* child = calc.original;

        calc.autoSizingX = child->m_size.x.unit() == EGE_LAYOUT_AUTO;
        calc.autoSizingY = child->m_size.y.unit() == EGE_LAYOUT_AUTO;
        calc.spacing = lms.spacing;

        // Ensure that parent is properly set
        child->m_parent = this;

        // Calculate child's layout.
        child->m_layout = calc;
        child->calculateLayout();
    }
}

void LayoutElement::removeObject(LayoutElement* el)
{
    m_children.erase(std::find_if(m_children.begin(), m_children.end(), [el](LayoutElement* _el) {
        if(_el == el)
            return true;
        return false;
    }));
}

void LayoutElement::setGeometryNeedUpdate(bool val)
{
    Renderable::setGeometryNeedUpdate(val);
    ege_log.debug() << "LayoutElement::setGeometryNeedUpdate() for " << getId();
    for(auto element: m_children)
        element->setGeometryNeedUpdate();
}

void LayoutElement::updateGeometry(Renderer&)
{
    ege_log.debug() << "Geometry Update for " << getId();
    ege_log.debug() << "Run Layout Update!! " << getId();
    calculateLayout();
}

void LayoutElement::runLayoutUpdate()
{
    ege_log.debug() << "runLayoutUpdate() " << getId();
    if(m_parent)
        m_parent->runLayoutUpdate();
    else
        calculateLayout();
}

}

