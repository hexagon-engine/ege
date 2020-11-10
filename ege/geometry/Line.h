/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Shape.h"

namespace EGE
{

// Line of form Ax+By+C=0
class Line : public Shape
{
public:
    Line(double _A, double _B, double _C)
    : A(_A), B(_B), C(_C) {}

    virtual ~Line() {}

    virtual std::shared_ptr<ShapeSet> intersection(std::shared_ptr<Shape> other) const;
    virtual std::shared_ptr<ShapeSet> sum(std::shared_ptr<Shape> other) const;
    virtual bool contains(std::shared_ptr<Point> other) const;
    virtual bool isEmpty() const;
    virtual std::shared_ptr<Shape> copy() const;

    double A = 0.0;
    double B = 0.0;
    double C = 0.0;
};

}
