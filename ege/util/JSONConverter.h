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
    virtual bool in(InputStreamType& input, std::shared_ptr<Object>& object) const;
    virtual bool out(OutputStreamType& output, const Object& object) const;
};

}
