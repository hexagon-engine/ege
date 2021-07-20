#include <ege/core.h>
#include <ege/util.h>
#include <tests/Tests.h>

int main()
{
    {
        EGE::DataManager manager("res/config.json");
        EXPECT(!manager.error());
        EXPECT_EQUAL(manager.getValue("test").asBoolean().value(), true);
        manager.setValue("test2", EGE::Serializers::object(true));
        EXPECT(manager.save());
    }
    return 0;
}
