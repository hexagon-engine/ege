/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "Converter.h"

#include <sstream>

namespace EGE
{

class JSONConverter : public Converter<std::istringstream, std::ostringstream>
{
public:
    virtual bool in(std::istringstream& input, ObjectMap& object) const;
    virtual bool out(std::ostringstream& output, const ObjectMap& object) const;
};

}
