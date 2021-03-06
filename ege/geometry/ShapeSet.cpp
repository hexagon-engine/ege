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

#include "ShapeSet.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

ShapeSet::ShapeSet(const ShapeSet& shapeSet)
{
    for(auto shape: shapeSet.m_subShapes)
    {
        append(SharedPtr<Shape>(shape->copy()));
    }
}

SharedPtr<ShapeSet> ShapeSet::intersection(SharedPtr<Shape> other) const
{
    SharedPtr<ShapeSet> shapeSet = make<ShapeSet>();
    for(auto shape: m_subShapes)
    {
        shapeSet->append(shape->intersection(other));
    }
    return shapeSet;
}

SharedPtr<ShapeSet> ShapeSet::sum(SharedPtr<Shape> other) const
{
    SharedPtr<ShapeSet> shapeSet = make<ShapeSet>(*this);
    shapeSet->append(other);
    return shapeSet;
}

bool ShapeSet::contains(SharedPtr<Point> point) const
{
    for(auto shape: m_subShapes)
    {
        if(shape->contains(point))
            return true;
    }
    return false;
}

bool ShapeSet::isEmpty() const
{
    return m_subShapes.empty();
}

SharedPtr<Shape> ShapeSet::copy() const
{
    return make<ShapeSet>(*this);
}

void ShapeSet::append(SharedPtr<Shape> shape)
{
    if(!shape->isEmpty())
        m_subShapes.push_back(shape->copy());
}

}
