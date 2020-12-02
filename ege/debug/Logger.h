/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include <ostream>

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

class Logger
{
public:
    Logger(std::ostream& output, LogLevel level, std::string streamName = "LOG");
    ~Logger();

    template<class T>
    Logger& operator<<(const T& t)
    {
        m_stream << t;
        return *this;
    }

private:
    static std::string prefix(LogLevel level);
    static LogColor color(LogLevel level);
    static LogBackground background(LogLevel level);

    std::ostream& m_stream;
};

namespace Log
{

void setErrorStream(std::ostream& output);
void setLogStream(std::ostream& output);

}

Logger err(LogLevel level = LogLevel::Error);
Logger log(LogLevel level = LogLevel::Info);

}

using EGE::LogLevel;
using EGE::Logger;
using EGE::err;
using EGE::log;
