#include <testsuite/Tests.h>
#include <ege/util/Converter.h>
#include <ege/util/Equation.h>
#include <ege/util/EquationSystem.h>
#include <ege/util/JSONConverter.h>
#include <ege/util/Math.h>
#include <ege/util/Object.h>
#include <ege/util/ObjectFloat.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectList.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/ObjectSerializers.h>
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

TESTCASE(equations)
{
    EXPECT_EQUAL(EGE::Equation::linear(0, 0), EGE::EquationResult::Infinite);
    EXPECT_EQUAL(EGE::Equation::linear(0, 1), EGE::EquationResult::None);
    EXPECT_EQUAL(EGE::Equation::linear(1, 1), -1);
    EXPECT_EQUAL(EGE::Equation::linear(5, 8), -1.6);
    EXPECT_EQUAL(EGE::Equation::square(0, 0, 0), EGE::EquationResult::Infinite);
    EXPECT_EQUAL(EGE::Equation::square(0, 0, 1), EGE::EquationResult::None);
    EXPECT_EQUAL(EGE::Equation::square(1, 0, 0), 0);
    EXPECT_EQUAL(EGE::Equation::square(1, 0, 1), EGE::EquationResult::None);
    std::vector<double> array1 = {-1, 1};
    EXPECT_EQUAL(EGE::Equation::square(1, 0, -1), array1);
    EGE::Vec2d pA = {1, 3};
    EGE::Vec2d pB = {3, 7};
    EXPECT_EQUAL(EGE::LinearEquation(pA, pB).b(), 1);
    EXPECT_EQUAL(EGE::LinearEquation(pA, pB).a(), 2);
}

TESTCASE(equationSystems)
{
    // 2 solutions
    {
        // Normal
        EGE::EquationSystemResult result = EGE::EquationSystem::linear({{1, 1}, {1, -2}}, {3, 0});
        EXPECT_EQUAL(result.resultFor(0), 2);
        EXPECT_EQUAL(result.resultFor(1), 1);
    }

    {
        // No solutions
        EGE::EquationSystemResult result = EGE::EquationSystem::linear({{1, -1}, {2, -2}}, {1, -3});
        EXPECT_EQUAL(result.resultFor(0), EGE::EquationResult::None);
        EXPECT_EQUAL(result.resultFor(1), EGE::EquationResult::None);
    }

    {
        // Infinite solutions
        EGE::EquationSystemResult result = EGE::EquationSystem::linear({{1, 1}, {3, 3}}, {2, 6});
        EXPECT_EQUAL(result.resultFor(0), EGE::EquationResult::Infinite);
        EXPECT_EQUAL(result.resultFor(1), EGE::EquationResult::Infinite);
    }

    // 3 solutions
    {
        // Normal
        EGE::EquationSystemResult result = EGE::EquationSystem::linear({{2, -2, -2}, {5, 2, 3}, {-1, 3, 4}}, {-2, 8, 4});
        EXPECT_EQUAL(result.resultFor(0), 1);
        EXPECT_EQUAL(result.resultFor(1), 3);
        EXPECT_EQUAL(result.resultFor(2), -1);
    }

    {
        // No solutions
        // FIXME: this example is not working for some reason :(
        EGE::EquationSystemResult result = EGE::EquationSystem::linear({{1, 1, 1}, {2, 2, 2}, {4, 4, 4}}, {2, 5, 8});
        EXPECT_EQUAL(result.resultFor(0), EGE::EquationResult::None);
        EXPECT_EQUAL(result.resultFor(1), EGE::EquationResult::None);
        EXPECT_EQUAL(result.resultFor(2), EGE::EquationResult::None);
    }

    {
        // Infinite solutions
        EGE::EquationSystemResult result = EGE::EquationSystem::linear({{1, 1, 1}, {2, 2, 2}, {4, 4, 4}}, {2, 4, 8});
        EXPECT_EQUAL(result.resultFor(0), EGE::EquationResult::Infinite);
        EXPECT_EQUAL(result.resultFor(1), EGE::EquationResult::Infinite);
        EXPECT_EQUAL(result.resultFor(2), EGE::EquationResult::Infinite);
    }
}

TESTCASE(serializers)
{
    auto vec1 = EGE::Vec2d(123.4, 56.789);
    auto test1 = EGE::Serializers::fromVector2(vec1);
    std::cerr << test1->toString() << std::endl;
    EXPECT_EQUAL(test1->getObject("x").lock()->asFloat(), 123.4);
    EXPECT_EQUAL(test1->getObject("y").lock()->asFloat(), 56.789);
    EXPECT_EQUAL(EGE::Serializers::toVector2(test1), (EGE::Vec2d)vec1);

    auto vec2 = EGE::Vec3i(123, 456, 789);
    auto test2 = EGE::Serializers::fromVector3(vec2);
    std::cerr << test2->toString() << std::endl;
    EXPECT_EQUAL(test2->getObject("x").lock()->asFloat(), 123);
    EXPECT_EQUAL(test2->getObject("y").lock()->asFloat(), 456);
    EXPECT_EQUAL(test2->getObject("z").lock()->asFloat(), 789);
    EXPECT_EQUAL(EGE::Serializers::toVector3(test2), (EGE::Vec3d)vec2);
}

RUN_TESTS(util)
