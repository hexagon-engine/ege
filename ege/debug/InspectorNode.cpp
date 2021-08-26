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

#include <string>
#include <sstream>
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
    ege_log.debug() << "InspectorNode: \e[32mCreating node\e[0m: " << m_isnName;
    isnSetParent(parent);
}

InspectorNode::~InspectorNode()
{
    if(m_isnParent)
        m_isnParent->m_isnChildren.erase(this);
    m_isnParent = nullptr;
    for(auto child: m_isnChildren)
        child->m_isnParent = nullptr;
}

void InspectorNode::isnSetParent(InspectorNode* node)
{
    ege_log.debug() << "InspectorNode: \e[33mSetting parent\e[0m: " << m_isnName << " < " << (node ? node->m_isnName : "<null>");
    if(m_isnParent)
        m_isnParent->m_isnChildren.erase(this);
    m_isnParent = node;
    if(m_isnParent)
        m_isnParent->m_isnChildren.insert(this);
}

// TODO: Do not duplicate this and printObject() copy
enum class IndentMode
{
    None,
    Normal,
    Object,
    LastObject
};

static String indent(std::vector<IndentMode> modes)
{
    String out;
    for(IndentMode mode: modes)
    {
        switch(mode)
        {
            case IndentMode::None:
                out += "   "; // "   "
                break;
            case IndentMode::Normal:
                out += "\u2502  "; // "|  "
                break;
            case IndentMode::Object:
                out += "\u251c\u2500 "; // "|--"
                break;
            case IndentMode::LastObject:
                out += "\u2514\u2500 "; // "L--"
                break;
        }
    }
    return out;
}

String _isnDisplayImpl(const InspectorNode& node, std::vector<IndentMode> modes, bool isLast)
{
    String text;

    // Depth indent
    text += indent(modes);

    // Info
    text += "\e[1m" + node.isnName() + "\e[0m";
    String info = node.isnInfo();
    if(!info.empty())
        text += " (" + info + ")";
    text += '\n';

    // Child info
    size_t count = 0;
    for(auto& child: node.isnChildren())
    {
        bool localIsLast = count == node.isnChildren().size() - 1;
        auto newModes = modes;
        if(!newModes.empty())
            newModes.back() = isLast ? IndentMode::None : IndentMode::Normal;
        newModes.push_back(localIsLast ? IndentMode::LastObject : IndentMode::Object);
        text += _isnDisplayImpl(*child, newModes, localIsLast);
        count++;
        if(count > 10)
            break;
    }

    return text;
}

String InspectorNode::isnDisplay() const
{
    return _isnDisplayImpl(*this, {}, true);
}

Profiler* InspectorNode::getProfiler() const
{
    if(m_profiler)
        return m_profiler.get();
    
    auto parent = isnParent();
    if(parent)
        return parent->getProfiler();

    return nullptr;
}

void InspectorNode::createProfiler()
{
    ASSERT_WITH_MESSAGE(!getProfiler(), "Profiler can be created if there is no profiler from parent");
    m_profiler = std::make_unique<Profiler>();
}

void InspectorNode::destroyProfilerIfNeeded()
{
    if(m_profiler)
        m_profiler.reset();
}

void InspectorNode::destroyProfiler()
{
    ASSERT_WITH_MESSAGE(m_profiler, "Profiler can be destroyed if there if no profiler from parent");
    m_profiler.reset();
}

}

