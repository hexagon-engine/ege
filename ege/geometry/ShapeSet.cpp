/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "ShapeSet.h"

namespace EGE
{

ShapeSet::ShapeSet(const ShapeSet& shapeSet)
{
    for(auto shape: shapeSet.m_subShapes)
    {
        append(std::shared_ptr<Shape>(shape->copy()));
    }
}

std::shared_ptr<ShapeSet> ShapeSet::intersection(std::shared_ptr<Shape> other) const
{
    std::shared_ptr<ShapeSet> shapeSet = std::make_shared<ShapeSet>();
    for(auto shape: m_subShapes)
    {
        shapeSet->append(shape->intersection(other));
    }
    return shapeSet;
}

std::shared_ptr<ShapeSet> ShapeSet::sum(std::shared_ptr<Shape> other) const
{
    std::shared_ptr<ShapeSet> shapeSet = std::make_shared<ShapeSet>(*this);
    shapeSet->append(other);
    return shapeSet;
}

bool ShapeSet::contains(std::shared_ptr<Point> point) const
{
    bool b1 = false;
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

std::shared_ptr<Shape> ShapeSet::copy() const
{
    return std::make_shared<ShapeSet>(*this);
}

void ShapeSet::append(std::shared_ptr<Shape> shape)
{
    if(!shape->isEmpty())
        m_subShapes.push_back(shape->copy());
}

}
