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

#include "ObjectSerializers.h"

namespace EGE
{

namespace Serializers
{

std::shared_ptr<Object> object(float t)                { return make<ObjectFloat>(t); }
std::shared_ptr<Object> object(double t)               { return make<ObjectFloat>(t); }
std::shared_ptr<Object> object(long double t)          { return make<ObjectFloat>(t); }

std::shared_ptr<Object> object(long long t)            { return make<ObjectInt>(t); }
std::shared_ptr<Object> object(int t)                  { return make<ObjectInt>(t, ObjectInt::Type::Int); }
std::shared_ptr<Object> object(short t)                { return make<ObjectInt>(t, ObjectInt::Type::Short); }

std::shared_ptr<Object> object(unsigned long long t)   { return make<ObjectUnsignedInt>(t); }
std::shared_ptr<Object> object(unsigned int t)         { return make<ObjectUnsignedInt>(t, ObjectUnsignedInt::Type::Int); }
std::shared_ptr<Object> object(unsigned short t)       { return make<ObjectUnsignedInt>(t, ObjectUnsignedInt::Type::Short); }

std::shared_ptr<Object> object(std::string t)          { return make<ObjectString>(t); }

Vec2d toVector2(std::shared_ptr<ObjectMap> map)
{
    if(!map)
        return {};
    auto x = map->getObject("x").as<Float>().valueOr(0);
    auto y = map->getObject("y").as<Float>().valueOr(0);
    return Vec2d(x, y);
}

Vec3d toVector3(std::shared_ptr<ObjectMap> map)
{
    if(!map)
        return {};
    auto x = map->getObject("x").as<Float>().valueOr(0);
    auto y = map->getObject("y").as<Float>().valueOr(0);
    auto z = map->getObject("z").as<Float>().valueOr(0);
    return Vec3d(x, y, z);
}

} // Serializers

}
