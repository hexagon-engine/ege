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

#include <map>
#include <memory>
#include <stack>
#include <string>

#include <ege/util/Serializable.h>

namespace EGE
{

class Profiler : public Serializable
{
public:
    Profiler();
    virtual ~Profiler();

    void start();
    void end();
    std::string toString() const;

    // NOTE: RAII wrapper (ProfilerSectionStarter) is recommended for this!
    // empty name for root section
    void startSection(std::string const& name = "");
    void endSection();
    void endStartSection(std::string const&  name);

    virtual SharedPtr<ObjectMap> serialize() const;
    virtual bool deserialize(SharedPtr<ObjectMap>);

private:
    friend class ProfilerSectionStarter;

    void startSectionLL(std::string const& name);
    void endSectionLL();
    long long getTime() const;

    struct Section : public Serializable
    {
        StringMap<UniquePtr<Section>> m_subSections;
        long long m_time; //in ns
        String m_name;
        bool m_started = false;
        long long m_startTime;
        int m_depth = 0;

        Section* findSubSection(std::string const& name);
        void addSectionInfo(std::string& info, long long parentTime, long long rootTime) const;

        virtual SharedPtr<ObjectMap> serialize() const;
        virtual bool deserialize(SharedPtr<ObjectMap>);
    };
    Section m_root;
    std::stack<Section*> m_startedSections;
};

}
