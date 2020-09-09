/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Shape.h"

namespace EGE
{

class Point : public Shape
{
public:
    Point(double _x = 0, double _y = 0)
    : x(_x), y(_y) {}

    virtual ~Point() {}

    virtual std::shared_ptr<ShapeSet> intersection(std::shared_ptr<Shape> other) const;
    virtual std::shared_ptr<ShapeSet> sum(std::shared_ptr<Shape> other) const;
    virtual bool contains(std::shared_ptr<Point> other) const;
    virtual bool isEmpty() const;
    virtual std::shared_ptr<Shape> copy() const;

    double x = 0.0;
    double y = 0.0;
};

}
