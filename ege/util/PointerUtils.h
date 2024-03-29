/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#pragma once

#include "Types.h"

#include <memory>

namespace EGE
{

template<typename T, typename... Args>
inline SharedPtr<T> make(Args&&... args)
{
    //std::cerr << "EGE::make<" << typeName<T>() << ">(" << display(args...) << ")" << std::endl;
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
inline SharedPtr<T> wrap(T* ptr)
{
    return SharedPtr<T>(std::shared_ptr<T>(ptr));
}

template<typename T, typename... Args>
inline UniquePtr<T> makeUnique(Args&&... args)
{
    //std::cerr << "EGE::make<" << typeName<T>() << ">(" << display(args...) << ")" << std::endl;
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
inline UniquePtr<T> wrapUnique(T* ptr)
{
    return UniquePtr<T>(ptr);
}

}

using EGE::make;
using EGE::makeUnique;
using EGE::wrap;
using EGE::wrapUnique;
