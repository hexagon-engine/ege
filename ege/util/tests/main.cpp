#include <testsuite/Tests.h>
#include <ege/util/Converter.h>
#include <ege/util/Object.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/ObjectString.h>
#include <iostream>

TESTCASE(object)
{
    EGE::ObjectMap map;
    map.addObject("test", std::make_shared<EGE::ObjectString>("test"));
    map.addObject("test2", std::make_shared<EGE::ObjectString>("test"));
    auto map2 = std::make_shared<EGE::ObjectMap>();
    map2->addObject("test3", std::make_shared<EGE::ObjectString>("test44"));
    map.addObject("testObjects", map2);
    DEBUG_PRINT(map.getObject("test").lock()->toString().c_str());
    DEBUG_PRINT(((EGE::ObjectMap*)map.getObject("testObjects").lock().get())->getObject("test3").lock()->toString().c_str());
    DEBUG_PRINT(map.toString().c_str());
}

class MyConverter : public EGE::Converter<std::string>
{
public:
    virtual bool in(std::string& input, EGE::ObjectMap& object) const
    {
        object.addObject("myString", std::make_shared<EGE::ObjectString>(input));
        return true;
    }

    virtual bool out(std::string& output, const EGE::ObjectMap& object) const
    {
        auto myString = object.getObject("myString").lock();
        if(!myString)
            return false;

        output = myString->asString();
        return true;
    }
};

TESTCASE(converter)
{
    MyConverter converter;
    std::string myString;
    EGE::ObjectMap map, map2;
    map.addObject("myString", std::make_shared<EGE::ObjectInt>(1234));
    std::cerr << map.toString() << std::endl;
    myString << EGE::objectOut(map, converter);
    EXPECT_EQUAL(myString, "1234");
    myString = "test4443";
    myString >> EGE::objectIn(map2, converter);
    std::cerr << map2.toString() << std::endl;
    EXPECT_EQUAL(map2.getObject("myString").lock()->asString(), "test4443");
}

RUN_TESTS(util)
