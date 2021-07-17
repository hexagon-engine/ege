#include <testsuite/Tests.h>
#include <ege/util.h>
#include <fstream>
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
    std::cout << map.getObject("test").asString().valueOr("").c_str() << std::endl;
    std::cout << map.getObject("testObjects")
                .to<EGE::ObjectMap>()
                .valueOr({})
                ->getObject("test3")
                .asString()
                .valueOr("")
                .c_str() << std::endl;
    std::cout << map.toString().c_str() << std::endl;
    return 0;
}

class MyConverter : public EGE::Converter<std::string>
{
public:
    virtual bool in(std::string& input, EGE::SharedPtr<EGE::Object>& object) const
    {
        EGE::SharedPtr<EGE::ObjectMap> objectMap = make<EGE::ObjectMap>();
        objectMap->addObject("myString", make<EGE::ObjectString>(input));
        object = objectMap;
        return true;
    }

    virtual bool out(std::string& output, const EGE::Object& object) const
    {
        if(object.isMap())
        {
            EGE::ObjectMap& objectMap = (EGE::ObjectMap&)object;

            auto _myString = objectMap.getObject("myString");
            if(!_myString.exists()) return false;
            output = _myString.asString().valueOr("");
            return true;
        }
        else
        {
            return false;
        }
    }
};

TESTCASE(converter)
{
    MyConverter converter;
    std::string myString;
    EGE::ObjectMap map;
    EGE::SharedPtr<EGE::Object> map2;
    map.addObject("myString", make<EGE::ObjectInt>(1234));
    std::cerr << map.toString() << std::endl;
    myString << EGE::objectOut(map, converter);
    EXPECT_EQUAL(myString, "1234");
    myString = "test4443";
    myString >> EGE::objectIn(map2, converter);
    std::cerr << map2->toString() << std::endl;
    EGE::SharedPtr<EGE::ObjectMap> objectMap = std::static_pointer_cast<EGE::ObjectMap>(map2);
    EXPECT_EQUAL(objectMap->getObject("myString").asString().value(), "test4443");
    return 0;
}

TESTCASE(serialize_api)
{
    // Serialize
    EGE::SharedPtr<EGE::ObjectMap> map = make<EGE::ObjectMap>();
    map->addInt("test1", 1);
    map->addInt("test3", 3);
    map->addInt("test5", 5);

    // Deserialize
    EGE::MaxInt test1 = map->getObject("test1").asInt().valueOr(0);
    EGE::MaxInt test3 = map->getObject("test3").asInt().valueOr(1);
    EGE::MaxInt test5 = map->getObject("test5").asInt().valueOr(2);
    EGE::MaxInt test8 = map->getObject("test8").asInt().valueOr(3);

    DUMP(1, test1); EXPECT(test1 == 1);
    DUMP(1, test3); EXPECT(test3 == 3);
    DUMP(1, test5); EXPECT(test5 == 5);
    DUMP(1, test8); EXPECT(test8 == 3);

    return 0;
}

TESTCASE(merge)
{
    // simple merge
    auto map1 = make<EGE::ObjectMap>();
    map1->addString("object1", "value1");
    map1->addString("object2", "value2");
    std::cerr << map1->toString() << std::endl;

    auto map2 = make<EGE::ObjectMap>();
    map2->addString("object3", "value3");
    map2->addString("object4", "value4");
    std::cerr << map2->toString() << std::endl;

    std::cerr << map1->merge(map2)->toString() << std::endl;

    // deep merge
    auto subMap1 = make<EGE::ObjectMap>();
    subMap1->addString("SubObject1", "gggg");
    map1->addObject("sub", subMap1);
    std::cerr << map1->toString() << std::endl;

    auto subMap2 = make<EGE::ObjectMap>();
    subMap2->addString("SubObject2", "gggg");
    map2->addObject("sub", subMap2);
    std::cerr << map2->toString() << std::endl;

    std::cerr << map1->merge(map2)->toString() << std::endl;
    return 0;
}

TESTCASE(lists)
{
    EGE::SharedPtr<EGE::ObjectMap> _mapFirst = make<EGE::ObjectMap>();
    EGE::SharedPtr<EGE::ObjectList> _list = make<EGE::ObjectList>();
    _list->addObject(EGE::Serializers::object(0.0));
    _list->addObject(EGE::Serializers::object(154.0));
    auto map = make<EGE::ObjectMap>();
    map->addObject("test3", make<EGE::ObjectString>("test44"));
    _list->addObject(map);
    _mapFirst->addObject("Test", _list);
    std::cerr << _mapFirst->toString() << std::endl;
    return 0;
}

TESTCASE(vectors)
{
    EGE::Vec2d vec(4, 3);
    double dist = EGE::VectorOperations::distanceTo({}, vec);
    EXPECT_EQUAL(dist, 5);
    double angle = EGE::VectorOperations::angleTo({}, vec);
    EXPECT(angle - 53.1301 < 0.01);

    EGE::Vec2i vec1(4, 3);
    EGE::Vec2i vec2(3, 4);
    EXPECT_EQUAL(EGE::VectorOperations::length(vec1), EGE::VectorOperations::length(vec2));

    // Transforms
    EXPECT_EQUAL(EGE::VectorOperations::scale(vec1, 2), EGE::Vec2i(8, 6));
    auto rot = EGE::VectorOperations::rotate(vec1, 90);
    DUMP(1, rot.x);
    DUMP(1, rot.y);
    double v = EGE::VectorOperations::distanceTo(EGE::VectorOperations::rotate(vec1, 90), EGE::Vec2i(3, -4));
    DUMP(1, v);
    EXPECT(v < 0.01);
    return 0;
}

TESTCASE(_json)
{
    // from file
    std::ifstream file("res/objects/registry.json");
    if(!file.good())
        return 2;
    EGE::SharedPtr<EGE::Object> obj;
    if(!(file >> EGE::objectIn(obj, EGE::JSONConverter())))
        return 1;

    // generate
    std::cerr << EGE::objectOut(*obj, EGE::JSONConverter());

    for(EGE::Size s = 0; s < 10240; s++)
    {
        // parse
        std::istringstream str;
        str.str("{ \"test1\":{},\"test2\":   {  \"aaa\":\"bbb\", \"ccc\": [ 0, 4, 6, {\"AA\":\"BB\"}  ]  }   , \
                 \"test\"   :   \"aaaa\"          \n\t\n\r, \"blablaba\":-31.1444E+0003,  \"L::ttt=tttsy\": [ \
                 0,1,2,3,4,5,\n\r6,7,8,9,\"a\"   \n], \"N e x t Text\"            :\"\"\n\n\n\n, \"test66\":{},\
                \"test77\":   {  \"aaa\":\"bbb\", \"ccc\": [ 0, 4, 6, {\"AA\":\"BB\"}  ]  }, \"EscapeTest\": \"  \
                \\n\\tTest\\\"Test\\\"\\\\ \\\n\t\tTEST\", \"booltest\": true}");
        EGE::SharedPtr<EGE::Object> obj2;
        if(!(str >> EGE::objectIn(obj2, EGE::JSONConverter())))
        {
            std::cerr << "parse error" << std::endl;
            return 1;
        }

        // generate
        std::ostringstream str2;
        str2 << EGE::objectOut(*obj2, EGE::JSONConverter());
    }
    return 0;
}

TESTCASE(objectIntTypes)
{
    EGE::SharedPtr<EGE::ObjectMap> map = make<EGE::ObjectMap>();
    long long number    =    0x7EDCBA9876543210;

    map->addInt("b", number, EGE::ObjectInt::Type::Byte);  // 0x10
    map->addInt("s", number, EGE::ObjectInt::Type::Short); // 0x3210
    map->addInt("i", number, EGE::ObjectInt::Type::Int);   // 0x76543210
    map->addInt("l", number, EGE::ObjectInt::Type::Long);  // 0x7EDCBA9876543210

    map->addUnsignedInt("ub", number, EGE::ObjectUnsignedInt::Type::Byte);  // 0x10
    map->addUnsignedInt("us", number, EGE::ObjectUnsignedInt::Type::Short); // 0x3210
    map->addUnsignedInt("ui", number, EGE::ObjectUnsignedInt::Type::Int);   // 0x76543210
    map->addUnsignedInt("ul", number, EGE::ObjectUnsignedInt::Type::Long);  // 0x7EDCBA9876543210

    EXPECT_EQUAL(map->getObject("b").asInt().value(), 0x10);
    EXPECT_EQUAL(map->getObject("s").asInt().value(), 0x3210);
    EXPECT_EQUAL(map->getObject("i").asInt().value(), 0x76543210);
    EXPECT_EQUAL(map->getObject("l").asInt().value(), 0x7EDCBA9876543210);

    EXPECT_EQUAL(map->getObject("ub").asUnsignedInt().value(), 0x10);
    EXPECT_EQUAL(map->getObject("us").asUnsignedInt().value(), 0x3210);
    EXPECT_EQUAL(map->getObject("ui").asUnsignedInt().value(), 0x76543210);
    EXPECT_EQUAL(map->getObject("ul").asUnsignedInt().value(), 0x7EDCBA9876543210);
    return 0;
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
    return 0;
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
    return 0;
}

TESTCASE(serializers)
{
    auto vec1 = EGE::Vec2d(123.25, 56.125);
    auto test1 = EGE::Serializers::fromVector2(vec1);
    double x = test1->getObject("x").asFloat().value();
    double y = test1->getObject("y").asFloat().value();
    EXPECT_EQUAL(x, 123.25);
    EXPECT_EQUAL(y, 56.125);
    EXPECT_EQUAL(EGE::Serializers::toVector2(test1), (EGE::Vec2d)vec1);

    auto vec2 = EGE::Vec3i(123, 456, 789);
    auto test2 = EGE::Serializers::fromVector3(vec2);
    EXPECT_EQUAL(test2->getObject("x").asFloat().value(), 123);
    EXPECT_EQUAL(test2->getObject("y").asFloat().value(), 456);
    EXPECT_EQUAL(test2->getObject("z").asFloat().value(), 789);
    EXPECT_EQUAL(EGE::Serializers::toVector3(test2), (EGE::Vec3d)vec2);

    auto color1 = EGE::ColorRGBA(0.5, 0.5, 0.75, 0.25);
    auto test3 = EGE::Serializers::fromColorRGBA(color1);
    EXPECT_EQUAL(test3->getObject("r").asFloat().value(), 0.5);
    EXPECT_EQUAL(test3->getObject("g").asFloat().value(), 0.5);
    EXPECT_EQUAL(test3->getObject("b").asFloat().value(), 0.75);
    EXPECT_EQUAL(test3->getObject("a").asFloat().value(), 0.25);
    EXPECT_EQUAL(EGE::Serializers::toColorRGBA(test3), (EGE::ColorRGBA)color1);

    auto rect1 = EGE::RectD(10, 11, 12, 13);
    auto test4 = EGE::Serializers::fromRect(rect1);
    EXPECT_EQUAL(test4->getObject("pos").to<EGE::ObjectMap>().value()->getObject("x").asFloat().value(), 10);
    EXPECT_EQUAL(test4->getObject("pos").to<EGE::ObjectMap>().value()->getObject("y").asFloat().value(), 11);
    EXPECT_EQUAL(test4->getObject("size").to<EGE::ObjectMap>().value()->getObject("x").asFloat().value(), 12);
    EXPECT_EQUAL(test4->getObject("size").to<EGE::ObjectMap>().value()->getObject("y").asFloat().value(), 13);
    EXPECT_EQUAL(EGE::Serializers::toRect(test4), (EGE::RectD)rect1);
    return 0;
}

TESTCASE(system)
{
    std::cerr << "OS IMPL: " << EGE::System::name() << std::endl;
    EXPECT(EGE::System::unixTime() > 1500000000);
    EXPECT(EGE::System::exactTime().seconds() > 1500000000);
    size_t i1 = EGE::System::getWorkingDirectory().size();
    EXPECT(i1 > 3);
    EGE::System::setWorkingDirectory("..");
    EXPECT(EGE::System::getWorkingDirectory().size() < i1);
    auto file = EGE::System::stat(EGE::System::getWorkingDirectory());
    EXPECT(file.exists() && file.type == EGE::System::FileType::Directory);
    EXPECT(EGE::System::testFileAccess("."));
    EXPECT(!EGE::System::testFileAccess("/?/nonexistent/"));
    DUMP(1, EGE::System::getErrorMessage());
    EGE::System::createPath("test/1/23/456");
    EGE::System::createPath("test2/1/23/456/");
    EGE::System::removePath("test");
    EGE::System::removePath("test2");
    std::cerr << "PATH: " << EGE::System::getEnv("PATH") << std::endl;
    return 0;
}

TESTCASE(commonPaths)
{
    std::cerr << "System paths:" << std::endl;
    std::cerr << "* currentDir = " << EGE::CommonPaths::currentDir() << std::endl;
    std::cerr << "* rootDir = " << EGE::CommonPaths::rootDir() << std::endl;
    std::cerr << "* homeDir = " << EGE::CommonPaths::homeDir() << std::endl;
    std::cerr << "* dataDir = " << EGE::CommonPaths::dataDir() << std::endl;
    std::cerr << "* installDir = " << EGE::CommonPaths::installDir() << std::endl;
    std::cerr << "Engine-specific paths:" << std::endl;
    std::cerr << "* resourceDir = " << EGE::CommonPaths::resourceDir() << std::endl;
    std::cerr << "* configDir = " << EGE::CommonPaths::configDir() << std::endl;
    std::cerr << "* saveDir = " << EGE::CommonPaths::saveDir() << std::endl;
    return 0;
}

TESTCASE(random)
{
    EGE::Random random(1235);
    std::map<uint64_t, size_t> distribution;
    for(size_t s = 0; s < 1000000; s++)
    {
        EXPECT(EGE::Random::fastRandom().nextInt(10) < 10);

        EXPECT(random.nextInt(10) < 10);
        EXPECT(random.nextFloat(10.0) < 10.0);
        EXPECT(random.nextDouble(10.0) < 10.0);

        // Ranged
        int64_t r1 = random.nextIntRanged(10, 5);
        distribution[r1]++;
        EXPECT(r1 <= 10 && r1 >= 5);
        float r2 = random.nextFloatRanged(10, 5);
        EXPECT(r2 <= 10 && r2 >= 5);
        double r3 = random.nextDoubleRanged(10, 5);
        EXPECT(r3 <= 10 && r3 >= 5);
    }
    // FIXME: it shows only even numbers, it's a disadvantage of bad LCG generator :(
    for(size_t s = 5; s <= 10; s++)
        std::cout << s << ": " << distribution[s] << " cases" << std::endl;
    return 0;
}

TESTCASE(math)
{
    EXPECT_EQUAL(EGE::map(0.5, 0.1, 1.0, 1.0, 10.0), 5.0);
    EXPECT_EQUAL(EGE::map(1, -5, 5, 0, 10), 6);
    return 0;
}

int testFunc(int a, int b)
{
    return a - b;
}

TESTCASE(_perf_std_function)
{
    std::function<int(int, int)> test([](int a, int b) { return a + b; });
    for(EGE::Size size = 0; size < 10240000; size++)
    {
        test(size, size + 1);
    }
    return 0;
}

int func(int a, int b)
{
    return a + b;
}
TESTCASE(_perf_normal_function)
{
    for(EGE::Size size = 0; size < 10240000; size++)
    {
        func(size, size + 1);
    }
    return 0;
}

TESTCASE(_perf_normal_function_ptr)
{
    int(*test)(int, int) = func;

    for(EGE::Size size = 0; size < 10240000; size++)
    {
        test(size, size + 1);
    }
    return 0;
}

TESTCASE(colors)
{
    // Default constructor
    EGE::ColorRGBA color1;
    EXPECT(color1.r == 0 && color1.g == 0 && color1.b == 0 && color1.a == 1);

    // Default color
    EGE::ColorRGBA color2 = EGE::Colors::cyan;
    EXPECT(color2.r == 0 && color2.g == 1 && color2.b == 1 && color2.a == 1);

    EGE::ColorRGBA color3(0.5, 0.5, 0.5, 0.75);
    EXPECT(color3.r == 0.5 && color3.g == 0.5 && color3.b == 0.5 && color3.a == 0.75);

    EGE::ColorRGBA color4 = EGE::ColorRGBA::fromBytes(0, 255, 255, 255);
    EXPECT(color4.r == 0 && color4.g > 0.9 && color4.b > 0.9 && color4.a > 0.9);

    EGE::ColorRGBA color5 = EGE::ColorRGBA::fromHTML("#80ff46", 200);
    EXPECT(color5.r == 0.5 && color5.g > 0.9 && color5.b > 0.27 && color5.a > 0.78);
    return 0;
}

TESTCASE(rect)
{
    EGE::RectD rect;
    EXPECT(rect.empty());
    EGE::RectD rect2a(10, 10, 20, 20);
    EGE::RectD rect2b(20, 20, 20, 20);
    EGE::RectD rect2i(20, 20, 10, 10);
    EXPECT_EQUAL(rect2a.intersection(rect2b), rect2i);
    EXPECT_EQUAL(rect2a * rect2b, rect2i);
    EXPECT(rect2a.contains({20, 20}));
    EXPECT(!rect2a.contains({9, 9}));
    EXPECT(!rect2a.contains({31, 31}));
    EXPECT(rect2a.contains({30, 30}));
    EXPECT(rect2a.contains({10, 10}));
    return 0;
}

TESTCASE(stream_print)
{
    std::cout << EGE::RectD() << ", " << EGE::ColorRGBA() << ", " << EGE::Vec2d() << ", " << EGE::PolVec2d() << std::endl;
    return 0;
}

RUN_TESTS(util)
