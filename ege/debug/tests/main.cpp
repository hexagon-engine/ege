#include <testsuite/Tests.h>
#include <ege/debug/Dump.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/JSONConverter.h>
#include <sstream>
#include <iostream>

TESTCASE(printObject)
{
    // parse
    std::istringstream str;
    str.str("{ \"test1\":{},\"test2\":   {  \"aaa\":\"bbb\", \"ccc\": [ 0, 4, 6, {\"AA\":\"BB\"}  ]  }    \
             \"test\"   :   \"aaaa\"          \n\t\n\r, \"blablaba\":-31.1444E+0003,  \"L::ttt=tttsy\": [ \
             0,1,2,3,4,5,\n\r6,7,8,9,\"a\"   \n], \"N e x t Text\"            :\"\"\n\n\n\n, \"test66\":{},\
            \"test77\":   {  \"aaa\":\"bbb\", \"ccc\": [ 0, 4, 6, {\"AA\":\"BB\"}  ]  }, \"EscapeTest\": \"  \
            \\n\\tTest\\\"Test\\\"\\\\ \\\n\t\tTEST\"}");
    std::shared_ptr<EGE::ObjectMap> map = make<EGE::ObjectMap>();
    if(!(str >> EGE::objectIn(*map.get(), EGE::JSONConverter())))
        std::cerr << "parse error" << std::endl;

    EGE::printObject(map);
}

RUN_TESTS(debug)
