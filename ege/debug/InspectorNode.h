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

#include <ege/debug/Profiler.h>
#include <ege/util/Types.h>

namespace EGE
{

class Inspector;

// Inspector node. It's NOT intended to keep object hierarchy.
class InspectorNode
{
public:
    InspectorNode(String name = "<BaseNode>");
    InspectorNode(InspectorNode* parent, String name = "<UnknownNode>");
    virtual ~InspectorNode();

    InspectorNode(const InspectorNode& node) = delete;
    InspectorNode(InspectorNode&& node) = delete;

    // Non-conventional names because this type is very
    // common to inherit and prone to ambiguity of
    // methods and fields.
    InspectorNode* isnParent() const { return m_isnParent; }
    String isnName() const { return m_isnName; }
    virtual String isnDisplay() const;
    void isnSetParent(InspectorNode*);
    virtual String isnInfo() const { return ""; }
    Set<InspectorNode*> const& isnChildren() const { return m_isnChildren; }

    Profiler* getProfiler() const;

protected:
    void createProfiler();
    void destroyProfiler();
    void destroyProfilerIfNeeded();

private:
    InspectorNode* m_isnParent = nullptr;
    String m_isnName;
    Set<InspectorNode*> m_isnChildren;
    UniquePtr<Profiler> m_profiler;
};

}
