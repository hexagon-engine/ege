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

#include <ege/util/ObjectMap.h>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace EGE
{

struct HexDumpSettings
{
    size_t width = 16;
    std::ostream& stream = std::cerr;
};

void hexDump(const void* data, size_t size, HexDumpSettings settings = HexDumpSettings());

void printObject(SharedPtr<Object> object);

struct PrintTreeConfig
{
    size_t maxDisplayedNodes = 10;
    size_t maxDepth = 10;
};

template<class ConstIterator>
class ContainerWrapper
{
public:
    ContainerWrapper(ConstIterator begin, ConstIterator end, size_t size)
    : m_begin(begin), m_end(end), m_size(size) {}

    ConstIterator begin() const { return m_begin; }
    ConstIterator end() const { return m_end; }
    size_t size() const { return m_size; }

private:
    ConstIterator m_begin;
    ConstIterator m_end;
    size_t m_size;
};

namespace Internal
{

enum class _IndentMode
{
    None,
    Normal,
    Object,
    LastObject
};

String _indent(std::vector<_IndentMode> modes);

// LeafToString: String leafToString(T const&)
// LeafToContainer: ContainerWrapper<It> leafToContainer(T const&)
template<class T, class It, class LeafToString, class LeafToContainer>
void _printTreeImpl(std::ostream& stream, T const& object, LeafToString leafToString, LeafToContainer leafToContainer, PrintTreeConfig const& config, std::vector<_IndentMode> modes, bool isLast)
{
    // Depth indent
    stream << _indent(modes);

    // Info
    stream << leafToString(object) << std::endl;

    // Child info
    size_t count = 0;

    auto objectAsContainer = leafToContainer(object);
    for(auto it = objectAsContainer.begin(); it != objectAsContainer.end(); it++)
    {
        // FIXME: Don't copy here if possible
        auto child = *it;
        bool localIsLast = count == objectAsContainer.size() - 1;
        auto newModes = modes;
        if(!newModes.empty())
            newModes.back() = isLast ? _IndentMode::None : _IndentMode::Normal;
        newModes.push_back(localIsLast ? _IndentMode::LastObject : _IndentMode::Object);

        if(modes.size() >= config.maxDepth)
        {
            newModes.back() = _IndentMode::LastObject;
            stream << _indent(newModes) << "(max depth exceeded)" << std::endl;
            return;
        }
        if(count > config.maxDisplayedNodes)
        {
            newModes.back() = _IndentMode::LastObject;
            stream << _indent(newModes) << "(skipped " << objectAsContainer.size() - config.maxDisplayedNodes << " objects)" << std::endl;
            return;
        }

        _printTreeImpl<T, It, LeafToString, LeafToContainer>(stream, child, leafToString, leafToContainer, config, newModes, localIsLast);
        count++;
    }
}

}

// LeafToString: template<class U> String leafToString(T const&)
// LeafToContainer: template<class U> _ContainerWrapper<It> leafToContainer(T const&)
template<class T, class It, class LeafToString, class LeafToContainer>
void printTree(std::ostream& stream, T const& object, LeafToString&& leafToString, LeafToContainer&& leafToContainer, PrintTreeConfig const& config = {})
{
    Internal::_printTreeImpl<T, It, LeafToString, LeafToContainer>(stream, object, std::move(leafToString), std::move(leafToContainer), config, {}, true);
}

}
