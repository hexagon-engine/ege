/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <memory>

namespace EGE
{

class Point;
class ShapeSet;

class Shape
{
public:
    virtual std::shared_ptr<ShapeSet> intersection(std::shared_ptr<Shape> other) const = 0;
    virtual std::shared_ptr<ShapeSet> sum(std::shared_ptr<Shape> other) const = 0;
    virtual bool contains(std::shared_ptr<Point> other) const = 0;
    virtual bool isEmpty() const = 0;
    virtual std::shared_ptr<Shape> copy() const = 0;
};

}
