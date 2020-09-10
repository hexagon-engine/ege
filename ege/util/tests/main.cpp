#include <testsuite/Tests.h>
#include <ege/util/Converter.h>
#include <ege/util/Object.h>
#include <ege/util/ObjectFloat.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectList.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/ObjectString.h>
#include <ege/util/Vector.h>
#include <ege/util/VectorOperations.h>
#include <iostream>

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

RUN_TESTS(util)
