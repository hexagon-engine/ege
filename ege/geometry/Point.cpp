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
    std::shared_ptr<ShapeSet> shapeSet = std::make_shared<ShapeSet>();

    if(other->contains(std::make_shared<Point>(*this)))
        shapeSet->append(std::make_shared<Point>(*this));

    return shapeSet;
}

std::shared_ptr<ShapeSet> Point::sum(std::shared_ptr<Shape> other) const
{
    std::shared_ptr<ShapeSet> shapeSet = std::make_shared<ShapeSet>();
    shapeSet->append(other);

    if(!other->contains(std::make_shared<Point>(*this)))
        shapeSet->append(std::make_shared<Point>(*this));

    return shapeSet;
}

bool Point::contains(std::shared_ptr<Point> other) const
{
    ASSERT(other)
    return other->x == x && other->y == y;
}

bool Point::isEmpty() const
{
    return false;
}

std::shared_ptr<Shape> Point::copy() const
{
    return std::make_shared<Point>(*this);
}

}
