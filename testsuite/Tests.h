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

#include <cstdio>
#include <ctime>
#include <functional>
#include <iostream>
#include <map>
#include <string>

class Tests
{
    std::map<std::string, std::function<int()>> m_tests;
    bool m_failed = false;
    bool m_currentFailed = false;
    std::string m_current;

public:
    void fail();
    int run();
    void add(std::string name, std::function<int()> test);
    std::string current();
};

void Tests::fail()
{
    m_currentFailed = true;
}

int Tests::run()
{
    for(auto& test: m_tests)
    {
        m_current = test.first;
        fprintf(stderr, "* Test: \e[34;1m%s\e[0m\n", m_current.c_str());

        clock_t _startTime = clock();
        int rc = test.second();
        clock_t _time = clock() - _startTime;

        if(m_currentFailed)
        {
            m_failed = true;
            if(rc == 0)
                rc = 1;
        }
        fprintf(stderr, "  \e[1m%s\e[0m (rc = \e[1m%d\e[0m, cpu time = \e[1m%f\e[0m ms)\n",  (!rc && !m_currentFailed) ? "\e[32mPASSED\e[0m" : "\e[31mFAILED\e[0m", rc, (_time * 1000 / ((double)CLOCKS_PER_SEC)));
        m_currentFailed = false;
    }
    return m_failed;
}

void Tests::add(std::string name, std::function<int()> test)
{
    m_tests.insert(std::make_pair(name, test));
}

std::string Tests::current()
{
    return m_current;
}

Tests tests;

#define TEST_ASSERT(a) \
do {\
    if(!(a)) \
    { \
        fprintf(stderr, "\e[31;1mASSERTION FAILED:\e[0m [ %s ] -- In \e[33m%s\e[0m:\e[95m%d\e[0m {%s}\n", #a, __FILE__, __LINE__, tests.current().c_str()); \
        tests.fail(); \
    } \
} while(0);

#define EXPECT(something) TEST_ASSERT(something)
#define EXPECT_EQUAL(_1, _2) EXPECT((_1) == (_2))

#define FAIL_TEST() tests.fail()

#define TESTCASE(name) \
    int _##name##_test(); \
    struct Test_##name \
    { \
        Test_##name() \
        { \
            tests.add(#name, _##name##_test); \
        } \
    } test_instance_##name; \
    int _##name##_test()

#define RUN_TESTS(name) \
    int main() \
    { \
        fprintf(stderr, "\e[35mTESTSUITE: \e[1m%s\e[0m\n", #name); \
        return tests.run(); \
    }
