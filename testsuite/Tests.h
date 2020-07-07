#pragma once

#include <cstdio>
#include <map>
#include <string>

typedef void(*Test)();

class Tests
{
    std::map<std::string, Test> m_tests;
    bool m_failed;
    std::string m_current;
public:
    void fail() { m_failed = true; }

    int run()
    {
        for(auto& test: m_tests)
        {
            m_current = test.first;
            test.second();
        }
        return m_failed;
    }

    void add(std::string name, Test test) { m_tests.insert(std::make_pair(name, test)); }
    std::string current() { return m_current; }
};

Tests tests;

#define TEST_ASSERT(a) \
do {\
    fprintf(stderr, "\e[33mTEST_ASSERT:\e[0m [ %s ] in %s\n", #a, tests.current().c_str()); \
    if(!(a)) \
    { \
        fprintf(stderr, "\e[31;1mASSERTION FAILED:\e[0m [ %s ] -- In \e[33m%s\e[0m:\e[95m%d\e[0m {%s}\n", #a, __FILE__, __LINE__, tests.current().c_str()); \
        tests.fail(); \
        return; \
    } \
} while(0);

#define EXPECT(something) TEST_ASSERT(something)
#define EXPECT_EQUAL(_1, _2) EXPECT((_1) == (_2))

#define TESTCASE(name) \
    void _##name##_test(); \
    struct Test_##name \
    { \
        Test_##name() \
        { \
            tests.add(#name, _##name##_test); \
        } \
    } test_instance_##name; \
    void _##name##_test()

#define RUN_TESTS(name) \
    int main() \
    { \
        fprintf(stderr, "\e[35mTESTSUITE: \e[1m%s\e[0m\n", #name); \
        return tests.run(); \
    }

#define DEBUG_PRINT(str) \
    fprintf(stderr, "  \e[36mdbg:\e[0m %s\n", str)
