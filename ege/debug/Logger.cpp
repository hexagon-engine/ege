/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Logger.h"

#include <ege/main/Config.h>
#include <iostream>

namespace EGE
{

std::ostream& operator<<(std::ostream& input, LogColor color)
{
    // TODO: make it portable
    switch(color)
    {
        case LogColor::Black:
            input << "\e[30m";
            break;
        case LogColor::Red:
            input << "\e[31m";
            break;
        case LogColor::Green:
            input << "\e[32m";
            break;
        case LogColor::Yellow:
            input << "\e[33m";
            break;
        case LogColor::Blue:
            input << "\e[34m";
            break;
        case LogColor::Magenta:
            input << "\e[35m";
            break;
        case LogColor::Cyan:
            input << "\e[36m";
            break;
        case LogColor::White:
            input << "\e[37m";
            break;
        case LogColor::Bright_Black:
            input << "\e[90m";
            break;
        case LogColor::Bright_Red:
            input << "\e[91m";
            break;
        case LogColor::Bright_Green:
            input << "\e[92m";
            break;
        case LogColor::Bright_Yellow:
            input << "\e[93m";
            break;
        case LogColor::Bright_Blue:
            input << "\e[94m";
            break;
        case LogColor::Bright_Magenta:
            input << "\e[95m";
            break;
        case LogColor::Bright_Cyan:
            input << "\e[96m";
            break;
        case LogColor::Bright_White:
            input << "\e[97m";
            break;
        case LogColor::Reset:
            input << "\e[39m";
            break;
    }
    return input;
}

std::ostream& operator<<(std::ostream& input, LogBackground color)
{
    // TODO: make it portable
    switch(color)
    {
        case LogBackground::Black:
            input << "\e[40m";
            break;
        case LogBackground::Red:
            input << "\e[41m";
            break;
        case LogBackground::Green:
            input << "\e[42m";
            break;
        case LogBackground::Yellow:
            input << "\e[43m";
            break;
        case LogBackground::Blue:
            input << "\e[44m";
            break;
        case LogBackground::Magenta:
            input << "\e[45m";
            break;
        case LogBackground::Cyan:
            input << "\e[46m";
            break;
        case LogBackground::White:
            input << "\e[47m";
            break;
        case LogBackground::Bright_Black:
            input << "\e[100m";
            break;
        case LogBackground::Bright_Red:
            input << "\e[101m";
            break;
        case LogBackground::Bright_Green:
            input << "\e[102m";
            break;
        case LogBackground::Bright_Yellow:
            input << "\e[103m";
            break;
        case LogBackground::Bright_Blue:
            input << "\e[104m";
            break;
        case LogBackground::Bright_Magenta:
            input << "\e[105m";
            break;
        case LogBackground::Bright_Cyan:
            input << "\e[106m";
            break;
        case LogBackground::Bright_White:
            input << "\e[107m";
            break;
        case LogBackground::Reset:
            input << "\e[49m";
            break;
    }
    return input;
}

Logger::Logger(std::ostream& output, LogLevel level, std::string streamName)
: m_stream(output)
{
    m_stream << LogColor::Magenta << "EGE/" << streamName << " " << LogColor::Bright_Yellow << prefix(level) << color(level) << background(level);
}

Logger::~Logger()
{
    m_stream << LogColor::Reset << LogBackground::Reset << std::endl;
}

LogColor Logger::color(LogLevel level)
{
    switch(level)
    {
        case LogLevel::Debug:
            return LogColor::Bright_Black;
        case LogLevel::Verbose:
            return LogColor::Bright_Blue;
        case LogLevel::Info:
            return LogColor::White;
        case LogLevel::Notice:
            return LogColor::Cyan;
        case LogLevel::Warning:
            return LogColor::Yellow;
        case LogLevel::Error:
            return LogColor::Red;
        case LogLevel::Critical:
            return LogColor::Bright_Red;
        case LogLevel::Crash:
            return LogColor::White;
        default:
            CRASH();
            return LogColor::Black;
    }
}

LogBackground Logger::background(LogLevel level)
{
    switch(level)
    {
        case LogLevel::Debug:
        case LogLevel::Verbose:
        case LogLevel::Info:
        case LogLevel::Notice:
        case LogLevel::Warning:
        case LogLevel::Error:
        case LogLevel::Critical:
            return LogBackground::Reset;
        case LogLevel::Crash:
            return LogBackground::Bright_Red;
        default:
            CRASH();
    }
}

std::string Logger::prefix(LogLevel level)
{
    std::string prefix;

    switch(level)
    {
        case LogLevel::Debug:
            prefix += "D";
            break;
        case LogLevel::Verbose:
            prefix += "V";
            break;
        case LogLevel::Info:
            prefix += "I";
            break;
        case LogLevel::Notice:
            prefix += "N";
            break;
        case LogLevel::Warning:
            prefix += "W";
            break;
        case LogLevel::Error:
            prefix += "E";
            break;
        case LogLevel::Critical:
            prefix += "C";
            break;
        case LogLevel::Crash:
            prefix += "R";
            break;
        default:
            CRASH();
    }

    prefix += ": ";
    return prefix;
}

namespace Log
{

std::ostream* g_errorStream = &std::cerr;
std::ostream* g_logStream = &std::cout;

void setErrorStream(std::ostream& output)
{
    g_errorStream = &output;
}

void setLogStream(std::ostream& output)
{
    g_logStream = &output;
}

} // Log

Logger err(LogLevel level)
{
    Logger logger(*Log::g_errorStream, level, "STDERR");
    return logger;
}

Logger log(LogLevel level)
{
    Logger logger(*Log::g_logStream, level, "STDOUT");
    return logger;
}

} // EGE
