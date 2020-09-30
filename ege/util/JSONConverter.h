/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Converter.h"

#include <iostream>

namespace EGE
{

class JSONConverter : public Converter<std::istream, std::ostream>
{
public:
    virtual bool in(InputStreamType& input, ObjectMap& object) const;
    virtual bool out(OutputStreamType& output, const ObjectMap& object) const;
};

}
