#include <testsuite/Tests.h>
#include <ege/gpo/GameplayObject.h>
#include <ege/gpo/GameplayObjectManager.h>
#include <ege/gpo/GameplayObjectRegistry.h>

#include <ege/util/Object.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectMap.h>
#include <ege/util/ObjectString.h>
#include <ege/util/Types.h>

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

    virtual EGE::SharedPtr<EGE::ObjectMap> serialize() const
    {
        EGE::SharedPtr<EGE::ObjectMap> map = make<EGE::ObjectMap>();
        map->addInt("int", m_int);
        map->addString("string", m_string);
        return map;
    }

    virtual bool deserialize(EGE::SharedPtr<EGE::ObjectMap> obj)
    {
        m_int = obj->getObject("int").asInt().valueOr(0);
        m_string = obj->getObject("string").asString().valueOr("");
        return true;
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
        registerMyObject(makeUnique<MyObjectTest>("testObject", -123, "addd"));
        numeric = registerMyObject(makeUnique<MyObject>("unknownObject", 54, "araer"));
        m_myObjectRegistry.replace("testObject", nullptr);
        m_myObjectRegistry.freeze();
        return true;
    }

    bool clear()
    {
        return true;
    }

    virtual EGE::SharedPtr<EGE::ObjectMap> serialize() const
    {
        return nullptr;
    }

    virtual bool deserialize(EGE::SharedPtr<EGE::ObjectMap>)
    {
        // TODO because of EGE::GameplayObjectRegistry
        return true;
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
    EXPECT_EQUAL(manager.m_myObjectRegistry.findById("nonexistent"), nullptr);
    EXPECT_EQUAL(manager.m_myObjectRegistry.findByNumericId(manager.numeric)->getString(), "araer");
    EXPECT_EQUAL(manager.m_myObjectRegistry.findById("testObject"), nullptr);

    return 0;
}

RUN_TESTS(gpo)
