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

#include "CommonPaths.h"

#include "OS.h"
#include "system.h"

namespace EGE
{

namespace CommonPaths
{

// system
String currentDir()
{
    return System::getWorkingDirectory();
}
String rootDir() // '/' on Unix/Linux, 'VOLUME:\' on Windows
{
    #if defined(EGE_API_UNIX)
        return "/";
    #elif defined(EGE_API_WIN32)
        return currentDir().substr(0, 3); // 3 first letters of current directory -> "C:\"
    #else
        #error Invalid OS for rootDir()
    #endif
}

String homeDir() // $HOME on Unix/Linux, %userprofile% on Windows
{
    #if defined(EGE_API_UNIX)
        return System::getEnv("HOME");
    #elif defined(EGE_API_WIN32)
        return System::getEnv("userprofile");
    #else
        #error Invalid OS for homeDir()
    #endif
}

String dataDir() // ~/.local/share on Unix/Linux, %AppData% on Windows
{
    #if defined(EGE_API_UNIX)
        return homeDir() + "/.local/share";
    #elif defined(EGE_API_WIN32)
        return System::getEnv("AppData");
    #else
        #error Invalid OS for dataDir()
    #endif
}

String installDir() // /usr/share on Unix/Linux, %ProgramFiles% on Windows
{
    #if defined(EGE_API_UNIX)
        return "/usr/share";
    #elif defined(EGE_API_WIN32)
        return System::getEnv("ProgramFiles");
    #else
        #error Invalid OS for installDir()
    #endif
}

// engine
String g_resourceDir = "res";
String g_configDir = "config";
String g_saveDir = "saves";

String resourceDir() { return g_resourceDir; }
String configDir() { return g_configDir; }
String saveDir() { return g_saveDir; }

void setResourceDir(String path) { g_resourceDir = path; }
void setConfigDir(String path) { g_configDir = path; }
void setSaveDir(String path) { g_saveDir = path; }

}

}

