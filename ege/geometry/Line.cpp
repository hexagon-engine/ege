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

#include "Line.h"

#include "Point.h"
#include "ShapeSet.h"

#include <ege/util/EquationSystem.h>
#include <ege/util/Geometry.h>

namespace EGE
{

std::shared_ptr<ShapeSet> Line::intersection(std::shared_ptr<Shape> other) const
{
    if(!other)
        return nullptr;

    // With point
    auto point = std::dynamic_pointer_cast<Point>(other);
    if(point)
    {
        if(contains(point))
            return make<ShapeSet>(other);
        else
            return nullptr;
    }

    // With other line
    auto line = std::dynamic_pointer_cast<Line>(other);
    if(line)
    {
        auto relation = Geometry::lineRelation<double>(A, B, C, line->A, line->B, line->C);
        if(relation == Geometry::LineRelation::Equal)
            return make<ShapeSet>(copy());
        else if(relation == Geometry::LineRelation::NoRelation || relation == Geometry::LineRelation::Parallel)
            return nullptr;
        else
        {
            EquationSystemResult result = EquationSystem::linear({ {A, B}, {line->A, line->B} }, {C, line->C});
            return make<ShapeSet>(make<Point>(result.resultFor(0).results[0], result.resultFor(1).results[1]));
        }
    }

    // With anything other
    return nullptr;
}

std::shared_ptr<ShapeSet> Line::sum(std::shared_ptr<Shape> other) const
{
    if(!other)
        return make<ShapeSet>(copy());

    // With point
    {
        auto point = std::dynamic_pointer_cast<Point>(other);
        if(point)
        {
            if(contains(point))
                return make<ShapeSet>(copy());
            else
                goto shapeSet;
        }
    }

    // With other line
    {
        auto line = std::dynamic_pointer_cast<Line>(other);
        if(line)
        {
            if(Geometry::lineRelation<double>(A, B, C, line->A, line->B, line->C) == Geometry::LineRelation::Equal)
                return make<ShapeSet>(copy());
            else
                goto shapeSet;
        }
    }

    // With another shapes
    shapeSet:

    SharedPtr<ShapeSet> shapeSet = make<ShapeSet>();
    shapeSet->append(copy());
    shapeSet->append(other->copy());
    return make<ShapeSet>(shapeSet);
}

bool Line::contains(std::shared_ptr<Point> other) const
{
    return Geometry::distanceToLine(A, B, C, other->x, other->y) == 0;
}

bool Line::isEmpty() const
{
    return false;
}

std::shared_ptr<Shape> Line::copy() const
{
    return make<Line>(*this);
}

}
