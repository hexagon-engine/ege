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

#include "Dump.h"
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

String InspectorNode::isnDisplay() const
{
    std::ostringstream oss;
    using ConstIterator = decltype(m_isnChildren)::const_iterator;
    printTree<InspectorNode const*, ConstIterator>(oss, this, [](InspectorNode const* const& node)->String {
        String out;
        out = "\e[1m" + node->isnName() + "\e[0m";
        String info = node->isnInfo();
        if(!info.empty())
            return out + " (" + info + ")";
        return out;
    }, [](InspectorNode const* const& node)->ContainerWrapper<ConstIterator> {
        return {node->m_isnChildren.begin(), node->m_isnChildren.end(), node->m_isnChildren.size()};
    });

    return oss.str();
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

