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

#include "Dump.h"

#include <iomanip>

namespace EGE
{

void hexDump(const void* data, size_t size, HexDumpSettings settings)
{
    std::ostream& _str = settings.stream;

    for(size_t s = 0; s < size / settings.width + 1; s++)
    {
        _str << std::hex << std::setfill('0') << std::setw(8) << s * settings.width << "   ";

        // data as HEX DUMP
        for(size_t t = 0; t < settings.width; t++)
        {
            size_t off = s * settings.width + t;
            if(off < size)
            {
                unsigned char _data = ((unsigned char*)data)[off] & 0xFF;
                _str << std::hex << std::setfill('0') << std::setw(2) << (int)_data << " ";
            }
            else
            {
                _str << "   ";
            }
        }

        _str << "  ";

        // and as CHARACTERS
        for(size_t t = 0; t < settings.width; t++)
        {
            size_t off = s * settings.width + t;
            if(off < size)
            {
                unsigned char _data = ((unsigned char*)data)[off] & 0xFF;
                if(_data < 32 || _data >= 127)
                    _data = '.';
                _str << _data << " ";
            }
        }
        _str << std::dec << std::endl;
    }
}

namespace Internal
{

String _indent(::std::vector<_IndentMode> modes)
{
    String out;
    for(_IndentMode mode: modes)
    {
        switch(mode)
        {
            case _IndentMode::None:
                out += "   "; // "   "
                break;
            case _IndentMode::Normal:
                out += "\u2502  "; // "|  "
                break;
            case _IndentMode::Object:
                out += "\u251c\u2500 "; // "|--"
                break;
            case _IndentMode::LastObject:
                out += "\u2514\u2500 "; // "L--"
                break;
        }
    }
    return out;
}

}

void printObject(SharedPtr<Object> object)
{
    // FIXME: This probably can be simpler.
    if(!object)
        return;

    using Type = std::pair<String, SharedPtr<Object>>;

    struct ListOrMapIterator
    {
    public:
        ListOrMapIterator(ObjectList::ValueType::const_iterator it)
        : m_listIt(it), m_isList(true) {}

        ListOrMapIterator(ObjectMap::ValueType::const_iterator it)
        : m_mapIt(it), m_isList(false) {}

        ListOrMapIterator(std::nullptr_t)
        : m_isEmpty(true) {}

        bool operator==(ListOrMapIterator const& it) const
        {
            return (m_isEmpty && it.m_isEmpty) || (m_isList == it.m_isList && (m_isList ? m_listIt == it.m_listIt : m_mapIt == it.m_mapIt));
        }
        bool operator!=(ListOrMapIterator const& it) const
        {
            return !(*this == it);
        }

        ListOrMapIterator operator++(int)
        {
            auto tmp = *this;
            if(m_isList)
                m_listIt++;
            else
                m_mapIt++;
            return tmp;
        }

        Type operator*() const
        {
            if(m_isList)
                return std::make_pair("", *m_listIt);
            return *m_mapIt;
        }

    private:
        ObjectList::ValueType::const_iterator m_listIt;
        ObjectMap::ValueType::const_iterator m_mapIt;
        bool m_isList = false;
        bool m_isEmpty = false;
    };

    printTree<Type, ListOrMapIterator>(std::cout, Type("Root", object), [](Type const& object)->String {
        String out = "\e[1m" + object.first + (object.first.empty() ? "\e[0m" : "\e[0m: ");
        if(object.second->isMap())
        {
            auto objectPtrMap = Object::unsafeCast<ObjectMap>(object.second);
            return out + "Map <" + std::to_string(objectPtrMap->size()) + " entries>";
        }
        if(object.second->isList())
        {
            auto objectPtrList = Object::unsafeCast<ObjectList>(object.second);
            return out + "List <" + std::to_string(objectPtrList->size()) + " entries>";
        }
        return out + object.second->toString();
    }, [](Type const& object)->ContainerWrapper<ListOrMapIterator> {
        auto objectPtr = object.second;
        if(objectPtr->isList())
        {
            auto objectPtrList = Object::unsafeCast<ObjectList>(objectPtr);
            return {ListOrMapIterator(objectPtrList->begin()), ListOrMapIterator(objectPtrList->end()), objectPtrList->size()};
        }
        else if(objectPtr->isMap())
        {
            auto objectPtrMap = Object::unsafeCast<ObjectMap>(objectPtr);
            return {ListOrMapIterator(objectPtrMap->begin()), ListOrMapIterator(objectPtrMap->end()), objectPtrMap->size()};
        }
        return {nullptr, nullptr, 0};
    });
}

}
