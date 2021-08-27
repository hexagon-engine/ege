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

#include <ege/main/Config.h>
#include <ege/util/TrackedSharedPtr.h>
#include <sstream>

namespace EGE
{

namespace Internal
{

void _TrackedSharedPtrBase::printMessage(MessageType message, std::string_view type)
{
    if(getFlat() == 0)
        return;

    if(type.find("Event") == std::string::npos)
        return;

    std::cerr << "TrackedSharedPtr: reference to @" << getFlat() << " [T=" << type.substr(0, type.size() - 1) << "] was ";
    switch(message)
    {
        case DefaultConstructed:
            std::cerr << "default constructed";
            break;
        case AssignedNullptr:
            std::cerr << "nullptr assigned";
            break;
        case AssignedSharedPtr:
            std::cerr << "std::shared_ptr assigned";
            break;
        case AssignedWeakPtr:
            std::cerr << "std::weak_ptr assigned";
            break;
        case CopyAssigned:
            std::cerr << "copy assigned";
            break;
        case MoveAssigned:
            std::cerr << "move assigned";
            break;
        case CastAssigned:
            std::cerr << "cast assigned";
            break;
        case Destructed:
            std::cerr << "destructed";
            break;
        default:
            break;
    }
    auto ref = refCount();
    std::cerr << ", refcount=" << ref;
    if(message == Destructed && ref == 0)
    {
        std::cerr << " \e[1;31m(Destructing object)\e[0m";
    }
    std::cerr << std::endl;
}

}

}
