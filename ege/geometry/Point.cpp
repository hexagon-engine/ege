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

#include "Point.h"

#include "ShapeSet.h"

#include <ege/main/Config.h>
#include <ege/util/PointerUtils.h>

namespace EGE
{

std::shared_ptr<ShapeSet> Point::intersection(std::shared_ptr<Shape> other) const
{
    std::shared_ptr<ShapeSet> shapeSet = make<ShapeSet>();

    if(other->contains(make<Point>(*this)))
        shapeSet->append(make<Point>(*this));

    return shapeSet;
}

std::shared_ptr<ShapeSet> Point::sum(std::shared_ptr<Shape> other) const
{
    std::shared_ptr<ShapeSet> shapeSet = make<ShapeSet>();
    shapeSet->append(other);

    if(!other->contains(make<Point>(*this)))
        shapeSet->append(make<Point>(*this));

    return shapeSet;
}

bool Point::contains(std::shared_ptr<Point> other) const
{
    ASSERT(other);
    return other->x == x && other->y == y;
}

bool Point::isEmpty() const
{
    return false;
}

std::shared_ptr<Shape> Point::copy() const
{
    return make<Point>(*this);
}

}
