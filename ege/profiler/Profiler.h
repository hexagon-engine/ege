/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <map>
#include <memory>
#include <stack>
#include <string>

#define PROFILER_DEBUG 0

namespace EGE
{

class Profiler
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

private:
    void startSectionLL(std::string name);
    void endSectionLL();
    long long getTime();

    struct Section
    {
        std::map<std::string, std::shared_ptr<Section>> m_subSections;
        long long m_time; //in ns
        std::string m_name;
        bool m_started = false;
        long long m_startTime;
        int m_depth = 0;

        Section* findSubSection(std::string name);
        void addSectionInfo(std::string& info, long long parentTime, long long rootTime);
    };
    Section m_root;
    std::stack<Section*> m_startedSections;
};

}
