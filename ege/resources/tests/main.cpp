#include <testsuite/Tests.h>
#include <ege/resources/ResourceManager.h>

class MyResourceManager : public EGE::ResourceManager
{
public:
    virtual bool reload()
    {
        EXPECT(setResourcePath("root/res"));
        EXPECT(!setResourcePath("root/notexisting"));
        EXPECT(loadFontFromFile("font.ttf"));
        EXPECT(loadTextureFromFile("texture.png"));
        EXPECT(getFont("font.ttf"));
        EXPECT(!getFont("notexisting.ttf"));
        std::shared_ptr<sf::Texture> tex;
        EXPECT(tex = getTexture("texture.png"));
        EXPECT(!getTexture("notexisting.png"));
        setUnknownTexture(tex);
        EXPECT(getTexture("notexisting2.png") == tex);
        EXPECT(!getTexture("notexisting.png"));
        return true;
    }
};

TESTCASE(simple)
{
    MyResourceManager manager;
    EXPECT(manager.reload());
    EXPECT(!manager.isError());
}

RUN_TESTS(resources);
