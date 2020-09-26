/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

//#include "StringUtils.h"

namespace EGE
{

template<typename T, typename... Args>
inline std::shared_ptr<T> make(Args&&... args)
{
    //std::cerr << "EGE::make<" << typeName<T>() << ">(" << display(args...) << ")" << std::endl;
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}

using EGE::make;
