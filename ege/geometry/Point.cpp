/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Point.h"

#include "ShapeSet.h"

#include <ege/main/Config.h>

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
