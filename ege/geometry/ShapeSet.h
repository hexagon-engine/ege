/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Shape.h"

#include <vector>

namespace EGE
{

class ShapeSet : public Shape
{
public:
    ShapeSet() = default;
    ShapeSet(const ShapeSet& shapeSet);

    virtual std::shared_ptr<ShapeSet> intersection(std::shared_ptr<Shape> other) const;
    virtual std::shared_ptr<ShapeSet> sum(std::shared_ptr<Shape> other) const;
    virtual bool contains(std::shared_ptr<Point> point) const;
    virtual bool isEmpty() const;
    virtual std::shared_ptr<Shape> copy() const;

    virtual void append(std::shared_ptr<Shape> shape);
    virtual size_t size()
    {
        return m_subShapes.size();
    }

private:
    std::vector<std::shared_ptr<Shape>> m_subShapes;
};

}
