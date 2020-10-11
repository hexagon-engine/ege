#include <testsuite/Tests.h>
#include <ege/util/Converter.h>
#include <ege/util/JSONConverter.h>
#include <ege/util/Object.h>
#include <ege/util/ObjectFloat.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectList.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/ObjectString.h>
#include <ege/util/PointerUtils.h>
#include <ege/util/Vector.h>
#include <iostream>
#include <sstream>

TESTCASE(object)
{
    EGE::ObjectMap map;
    map.addObject("test", make<EGE::ObjectString>("test"));
    map.addObject("test2", make<EGE::ObjectString>("test"));
    auto map2 = make<EGE::ObjectMap>();
    map2->addObject("test3", make<EGE::ObjectString>("test44"));
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
        object.addObject("myString", make<EGE::ObjectString>(input));
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
    map.addObject("myString", make<EGE::ObjectInt>(1234));
    std::cerr << map.toString() << std::endl;
    myString << EGE::objectOut(map, converter);
    EXPECT_EQUAL(myString, "1234");
    myString = "test4443";
    myString >> EGE::objectIn(map2, converter);
    std::cerr << map2.toString() << std::endl;
    EXPECT_EQUAL(map2.getObject("myString").lock()->asString(), "test4443");
}

TESTCASE(merge)
{
    // simple merge
    auto map1 = make<EGE::ObjectMap>();
    map1->addObject("object1", make<EGE::ObjectString>("value1"));
    map1->addObject("object2", make<EGE::ObjectString>("value2"));
    std::cerr << map1->toString() << std::endl;

    auto map2 = make<EGE::ObjectMap>();
    map2->addObject("object3", make<EGE::ObjectString>("value3"));
    map2->addObject("object4", make<EGE::ObjectString>("value4"));
    std::cerr << map2->toString() << std::endl;

    std::cerr << map1->merge(map2)->toString() << std::endl;

    // deep merge
    auto subMap1 = make<EGE::ObjectMap>();
    subMap1->addObject("SubObject1", make<EGE::ObjectString>("gggg"));
    map1->addObject("sub", subMap1);
    std::cerr << map1->toString() << std::endl;

    auto subMap2 = make<EGE::ObjectMap>();
    subMap2->addObject("SubObject2", make<EGE::ObjectString>("gggg"));
    map2->addObject("sub", subMap2);
    std::cerr << map2->toString() << std::endl;

    std::cerr << map1->merge(map2)->toString() << std::endl;
}

TESTCASE(lists)
{
    std::shared_ptr<EGE::ObjectMap> _mapFirst = make<EGE::ObjectMap>();
    std::shared_ptr<EGE::ObjectList> _list = make<EGE::ObjectList>();
    _list->addObject(make<EGE::ObjectFloat>(0.0));
    _list->addObject(make<EGE::ObjectFloat>(154.0));
    auto map = make<EGE::ObjectMap>();
    map->addObject("test3", make<EGE::ObjectString>("test44"));
    _list->addObject(map);
    _mapFirst->addObject("Test", _list);
    std::cerr << _mapFirst->toString() << std::endl;
}

TESTCASE(vectors)
{
    EGE::Vec2d vec(4, 3);
    double dist = EGE::VectorOperations::distanceTo({}, vec);
    EXPECT_EQUAL(dist, 5);
    double angle = EGE::VectorOperations::angleTo(vec, vec);
    std::cerr << angle << std::endl;

    EGE::Vec2i vec1(4, 3);
    EGE::Vec2i vec2(3, 4);
    EXPECT_EQUAL(EGE::VectorOperations::length(vec1), EGE::VectorOperations::length(vec2));
    EXPECT(vec1 < vec2 * 2);
    EXPECT(vec1 * 2 > vec2);
    EXPECT(vec1 >= vec2);
    EXPECT(vec1 <= vec2);
}

TESTCASE(json)
{
    // parse
    std::istringstream str;
    str.str("{ \"test1\":{},\"test2\":   {  \"aaa\":\"bbb\", \"ccc\": [ 0, 4, 6, {\"AA\":\"BB\"}  ]  }   , \
             \"test\"   :   \"aaaa\"          \n\t\n\r, \"blablaba\":-31.1444E+0003,  \"L::ttt=tttsy\": [ \
             0,1,2,3,4,5,\n\r6,7,8,9,\"a\"   \n], \"N e x t Text\"            :\"\"\n\n\n\n, \"test66\":{},\
            \"test77\":   {  \"aaa\":\"bbb\", \"ccc\": [ 0, 4, 6, {\"AA\":\"BB\"}  ]  }, \"EscapeTest\": \"  \
            \\n\\tTest\\\"Test\\\"\\\\ \\\n\t\tTEST\"}");
    EGE::ObjectMap map;
    if(!(str >> EGE::objectIn(map, EGE::JSONConverter())))
        std::cerr << "parse error" << std::endl;

    // generate
    std::ostringstream str2;
    str2 << EGE::objectOut(map, EGE::JSONConverter());
    std::cerr << "generated=" << str2.str() << std::endl;
}

TESTCASE(objectIntTypes)
{
    std::shared_ptr<EGE::ObjectMap> map = make<EGE::ObjectMap>();
    long long signedNumber = 0xFEDCBA9876543210;
    long long number    =    0xFEDCBA9876543210;

    map->addInt("b", number, EGE::ObjectInt::Type::Byte);  // 0x10
    map->addInt("s", number, EGE::ObjectInt::Type::Short); // 0x3210
    map->addInt("i", number, EGE::ObjectInt::Type::Int);   // 0x76543210
    map->addInt("l", number, EGE::ObjectInt::Type::Long);  // 0xFEDCBA9876543210

    map->addUnsignedInt("ub", number, EGE::ObjectUnsignedInt::Type::Byte);  // 0x10
    map->addUnsignedInt("us", number, EGE::ObjectUnsignedInt::Type::Short); // 0x3210
    map->addUnsignedInt("ui", number, EGE::ObjectUnsignedInt::Type::Int);   // 0x76543210
    map->addUnsignedInt("ul", number, EGE::ObjectUnsignedInt::Type::Long);  // 0xFEDCBA9876543210

    EXPECT_EQUAL(map->getObject("b").lock()->asInt(), 0x10);
    EXPECT_EQUAL(map->getObject("s").lock()->asInt(), 0x3210);
    EXPECT_EQUAL(map->getObject("i").lock()->asInt(), 0x76543210);
    EXPECT_EQUAL(map->getObject("l").lock()->asInt(), 0xFEDCBA9876543210);

    EXPECT_EQUAL(map->getObject("ub").lock()->asUnsignedInt(), 0x10);
    EXPECT_EQUAL(map->getObject("us").lock()->asUnsignedInt(), 0x3210);
    EXPECT_EQUAL(map->getObject("ui").lock()->asUnsignedInt(), 0x76543210);
    EXPECT_EQUAL(map->getObject("ul").lock()->asUnsignedInt(), 0xFEDCBA9876543210);
}

RUN_TESTS(util)
