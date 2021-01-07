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

#include <ostream>
#include <set>

namespace EGE
{

enum class LogLevel
{
    Debug,
    Verbose,
    Info,
    Notice,
    Warning,
    Error,
    Critical,
    Crash
};

enum class LogColor
{
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,

    Bright_Black,
    Bright_Red,
    Bright_Green,
    Bright_Yellow,
    Bright_Blue,
    Bright_Magenta,
    Bright_Cyan,
    Bright_White,

    Reset
};

enum class LogBackground
{
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,

    Bright_Black,
    Bright_Red,
    Bright_Green,
    Bright_Yellow,
    Bright_Blue,
    Bright_Magenta,
    Bright_Cyan,
    Bright_White,

    Reset
};


std::ostream& operator<<(std::ostream& input, LogColor color);

namespace Internal
{

class _LoggerHelper
{
public:
    _LoggerHelper() {}
    _LoggerHelper(std::ostream* output, LogLevel level, std::string streamName);
    ~_LoggerHelper();

    template<class T>
    _LoggerHelper& operator<<(const T& t)
    {
        if(m_stream)
            *m_stream << t;
        return *this;
    }

private:
    static std::string prefix(LogLevel level);
    static LogColor color(LogLevel level);
    static LogBackground background(LogLevel level);

    std::ostream* m_stream = nullptr;
};

} // Internal

class Logger
{
public:
    Logger(std::ostream* output, std::string streamName = "LOG")
    : m_output(output), m_streamName(streamName) {}

    void setOutput(std::ostream* output) { m_output = output; }
    std::ostream* getOutput() { return m_output; }
    std::string getName() { return m_streamName; }
    void filterLevel(LogLevel level, bool filter = true);
    Internal::_LoggerHelper log(LogLevel level);

private:
    std::ostream* m_output;
    std::string m_streamName;
    std::set<LogLevel> m_filteredLevels = { LogLevel::Debug, LogLevel::Verbose };
};

Internal::_LoggerHelper err(LogLevel level = LogLevel::Error);
Internal::_LoggerHelper log(LogLevel level = LogLevel::Info);

Logger& errLogger();
Logger& mainLogger();

}

using EGE::LogLevel;
using EGE::Logger;
using EGE::err;
using EGE::log;
