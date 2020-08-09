#include <testsuite/Tests.h>
#include <ege/resources/ResourceManager.h>
#include <ege/main/Config.h>

class MyResourceManager : public EGE::ResourceManager
{
public:
    virtual bool reload()
    {
        EXPECT(setResourcePath("res"));
        EXPECT(!setResourcePath("notexisting"));
        EXPECT(loadFontFromFile("font.ttf"));
        EXPECT(loadTextureFromFile("texture.png"));
        EXPECT(getFont("font.ttf"));
        setDefaultFont("");
        EXPECT(!getFont("notexisting.ttf"));
        std::shared_ptr<sf::Texture> tex;
        EXPECT(tex = getTexture("texture.png"));
        EXPECT(!getTexture("notexisting.png"));
        setUnknownTexture(tex);
        EXPECT(getTexture("notexisting2.png") == tex);
        EXPECT(!getTexture("notexisting.png"));
        clear();
        addTexture("textureTest.png");
        addTexture("texture.png");
        EXPECT(!getTexture("textureTest.png"));
        EXPECT(getTexture("texture.png"));
        return true;
    }
};

class MyResourceManager2 : public EGE::ResourceManager
{
public:
    virtual bool reload()
    {
        EXPECT(!getFont("font.ttf"));
        EXPECT(setDefaultFont("font.ttf"));
        EXPECT(m_defaultFont == "font.ttf");
        EXPECT(getFont("font.ttf"));
        DUMP(1, getDefaultFont());
        DUMP(1, getFont("font.ttf"));
        EXPECT_EQUAL(getDefaultFont(), getFont("font.ttf"));
        EXPECT(getDefaultFont() == getFont("font3.ttf"));
        EXPECT(!setDefaultFont("font2.ttf"));
        return true;
    }
};

TESTCASE(simple)
{
    MyResourceManager manager;
    EXPECT(manager.reload());
    EXPECT(manager.isError());
}

TESTCASE(defaultFont)
{
    MyResourceManager2 manager;
    EXPECT(manager.reload());
    EXPECT(manager.isError());
}

RUN_TESTS(resources);
