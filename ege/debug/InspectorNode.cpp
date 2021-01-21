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

#include "InspectorNode.h"

#include "Inspector.h"


#include <cstring>  // for backtrace
#include <execinfo.h>  // for backtrace
#include <dlfcn.h>     // for dladdr
#include <string>
#include <sstream>
#include <cxxabi.h>    // for __cxa_demangle
#include <typeinfo>

#define KEEP_REMOVED_NODES 1

namespace EGE
{

InspectorNode::InspectorNode(String name)
: InspectorNode(&Inspector::instance().rootNode(), name)
{

}

InspectorNode::InspectorNode(InspectorNode* parent, String name)
: m_isnName(name)
{
    log(LogLevel::Debug) << "InspectorNode: \e[32mCreating node\e[0m: " << m_isnName;
    isnSetParent(parent);
}

InspectorNode::~InspectorNode()
{
    log(LogLevel::Debug) << "InspectorNode: \e[31mRemoving node\e[0m: " << m_isnName;

    if(m_isnParent)
        m_isnParent->m_isnChildren.erase(this);
    m_isnParent = nullptr;
    for(auto child: m_isnChildren)
        child->m_isnParent = nullptr;
}

void InspectorNode::isnSetParent(InspectorNode* node)
{
    if(m_isnParent)
        m_isnParent->m_isnChildren.erase(this);
    m_isnParent = node;
    if(m_isnParent)
        m_isnParent->m_isnChildren.insert(this);
}

String InspectorNode::isnDisplay(size_t depth) const
{
    String text;

    // Depth indent
    for(size_t s = 0; s < depth; s++)
        text += "  ";
    text += " * ";

    // Info
    text += "\e[1m" + m_isnName + "\e[0m";
    String info = isnInfo();
    if(!info.empty())
        text += " (" + info + ")";
    text += '\n';

    // Child info
    depth++;
    for(auto child: m_isnChildren)
        text += child->isnDisplay(depth);

    return text;
}

}

