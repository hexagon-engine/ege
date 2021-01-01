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

#define PROFILER_DEBUG 0

namespace EGE
{

class Profiler : public Serializable
{
public:
    Profiler();
    virtual ~Profiler();

    // empty name for root section
    void startSection(std::string name = "");
    void endSection();
    void endStartSection(std::string name);
    void start();
    void end();
    std::string toString();

    virtual std::shared_ptr<ObjectMap> serialize() const;
    virtual bool deserialize(std::shared_ptr<ObjectMap>);

private:
    void startSectionLL(std::string name);
    void endSectionLL();
    long long getTime();

    struct Section : public Serializable
    {
        std::map<std::string, std::shared_ptr<Section>> m_subSections;
        long long m_time; //in ns
        std::string m_name;
        bool m_started = false;
        long long m_startTime;
        int m_depth = 0;

        Section* findSubSection(std::string name);
        void addSectionInfo(std::string& info, long long parentTime, long long rootTime);

        virtual std::shared_ptr<ObjectMap> serialize() const;
        virtual bool deserialize(std::shared_ptr<ObjectMap>);
    };
    Section m_root;
    std::stack<Section*> m_startedSections;
};

}
