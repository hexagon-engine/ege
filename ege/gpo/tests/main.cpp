#include <testsuite/Tests.h>
#include <ege/gpo/GameplayObject.h>
#include <ege/gpo/GameplayObjectManager.h>
#include <ege/gpo/GameplayObjectRegistry.h>

#include <ege/util/Object.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/ObjectString.h>

class MyObject : public EGE::GameplayObject
{
    int m_int;
    std::string m_string;
public:
    MyObject(std::string id, int myInt, std::string myString)
    : EGE::GameplayObject(id)
    , m_int(myInt)
    , m_string(myString) {}

    std::string getString()
    {
        return m_string;
    }
    int getInt()
    {
        return m_int;
    }
    virtual std::string type()
    {
        return "unknown";
    }

    virtual std::shared_ptr<EGE::ObjectMap> serialize()
    {
        std::shared_ptr<EGE::ObjectMap> map = make<EGE::ObjectMap>();
        map->addObject("int", make<EGE::ObjectInt>(m_int));
        map->addObject("string", make<EGE::ObjectString>(m_string));
        return map;
    }

    virtual void deserialize(std::shared_ptr<EGE::ObjectMap> obj)
    {
        m_int = obj->getObject("int").lock()->asInt();
        m_string = obj->getObject("string").lock()->asString();
    }
};

class MyObjectTest : public MyObject
{
public:
    MyObjectTest(std::string id, int myInt, std::string myString)
    : MyObject(id, myInt, myString) {}

    virtual std::string type()
    {
        return "test";
    }
};

class MyGameplayObjectManager : public EGE::GameplayObjectManager
{
public:
    EGE::GameplayObjectRegistry<std::string, MyObject> m_myObjectRegistry;
    int numeric = 0;

    bool load()
    {
        registerMyObject(std::make_unique<MyObjectTest>("testObject", -123, "addd"));
        numeric = registerMyObject(std::make_unique<MyObject>("unknownObject", 54, "araer"));
        return true;
    }

    bool clear()
    {
        return true;
    }

    virtual std::shared_ptr<EGE::ObjectMap> serialize()
    {
        return nullptr;
    }

    virtual void deserialize(std::shared_ptr<EGE::ObjectMap>)
    {
        // TODO because of EGE::GameplayObjectRegistry
    }

private:
    int registerMyObject(EGE::UniquePtr<MyObject> object)
    {
        std::string id = object->getId();
        return (int)m_myObjectRegistry.add(id, std::move(object));
    }
};

TESTCASE(simple)
{
    MyGameplayObjectManager manager;
    manager.reload();
    EXPECT_EQUAL(manager.m_myObjectRegistry.findById("unknownObject")->getInt(), 54);
    EXPECT_EQUAL(manager.m_myObjectRegistry.findById("unknownObject")->type(), "unknown");
    EXPECT_EQUAL(manager.m_myObjectRegistry.findById("testObject")->type(), "test");
    EXPECT_EQUAL(manager.m_myObjectRegistry.findById("testObject")->getString(), "addd");
    EXPECT_EQUAL(manager.m_myObjectRegistry.findById("nonexistent"), nullptr);
    EXPECT_EQUAL(manager.m_myObjectRegistry.findByNumericId(manager.numeric)->getString(), "araer");
    manager.m_myObjectRegistry.remove("testObject");
    EXPECT_EQUAL(manager.m_myObjectRegistry.findById("testObject"), nullptr);

    return 0;
}

RUN_TESTS(gpo)
