#include <testsuite/Tests.h>
#include <ege/gpo/GameplayObject.h>
#include <ege/gpo/GameplayObjectManager.h>
#include <ege/gpo/GameplayObjectRegistry.h>

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
        registerMyObject(new MyObjectTest("testObject", -123, "addd"));
        numeric = registerMyObject(new MyObject("unknownObject", 54, "araer"));
        return true;
    }

    bool clear()
    {
        return true;
    }

private:
    int registerMyObject(MyObject* object)
    {
        return (int)m_myObjectRegistry.add(object->getId(), object);
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
}

RUN_TESTS(gpo)
