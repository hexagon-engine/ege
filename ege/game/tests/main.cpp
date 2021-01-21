#include <testsuite/Tests.h>
#include <ege/game/Game.h>
#include <ege/gui/Label.h>
#include <ege/gpo/GameplayObject.h>
#include <ege/gpo/GameplayObjectManager.h>
#include <ege/gpo/GameplayObjectRegistry.h>
#include <ege/util/ObjectInt.h>
#include <ege/util/ObjectMap.h>

class MyColor : public EGE::GameplayObject
{
public:
    MyColor(std::string id, sf::Color color)
    : EGE::GameplayObject(id)
    , m_color(color) {}

    sf::Color m_color;

    virtual std::shared_ptr<EGE::ObjectMap> serialize() const
    {
        std::shared_ptr<EGE::ObjectMap> map = make<EGE::ObjectMap>();
        map->addInt("r", m_color.r);
        map->addInt("g", m_color.g);
        map->addInt("b", m_color.b);
        map->addInt("a", m_color.a);
        return map;
    }

    virtual bool deserialize(std::shared_ptr<EGE::ObjectMap> obj)
    {
        m_color.r = obj->getObject("r").asInt().valueOr(0);
        m_color.g = obj->getObject("g").asInt().valueOr(0);
        m_color.b = obj->getObject("b").asInt().valueOr(0);
        m_color.a = obj->getObject("a").asInt().valueOr(0);
        return true;
    }
};

class MyGameplayObjectManager : public EGE::GameplayObjectManager
{
public:
    static MyGameplayObjectManager* instance;

    MyGameplayObjectManager()
    {
        instance = this;
    }

    EGE::GameplayObjectRegistry<std::string, MyColor> colors;

    virtual void registerObject(std::string id, int r, int g, int b)
    {
        colors.add(id, std::make_unique<MyColor>(id, sf::Color(r,g,b)));
    }

    virtual bool clear()
    {
        colors.clear();
        return true;
    }

    virtual bool load()
    {
        DEBUG_PRINT("MyGameplayObjectManager load");
        registerObject("black", 0, 0, 0);
        registerObject("red", 255, 0, 0);
        registerObject("green", 0, 255, 0);
        registerObject("blue", 0, 0, 255);
        registerObject("cyan", 0, 255, 255);
        registerObject("magenta", 255, 0, 255);
        registerObject("yellow", 255, 255, 0);
        registerObject("white", 255, 255, 255);
        colors.freeze();
        return true;
    }
};

MyGameplayObjectManager* MyGameplayObjectManager::instance;

class ColorWidget : public EGE::CompoundWidget
{
public:
    MyColor* m_color;
    typedef EGE::GameplayObjectRegistry<EGE::String, MyColor> GPORObjc;

    ColorWidget(EGE::Widget& parent, size_t index, MyColor* color)
    : EGE::CompoundWidget(parent)
    , m_color(color)
    {
        auto label = make<EGE::Label>(*this);
        label->setString(std::to_string(index) + ": " + color->getId());
        label->setFontSize(15);
        addWidget(label);
    }

    void render(EGE::Renderer& renderer) const override
    {
        sf::RectangleShape rs(sf::Vector2f(30.f, 30.f));
        rs.setPosition(1.f, 1.f);
        rs.setOutlineColor(sf::Color(255 - m_color->m_color.r, 255 - m_color->m_color.g, 255 - m_color->m_color.b));

        if(m_mouseOver)
            rs.setOutlineThickness(1.f);

        rs.setFillColor(m_color->m_color);
        renderer.getTarget().draw(rs);
    }
};

class MyGuiScreen : public EGE::GUIScreen
{
public:
    std::vector<std::shared_ptr<ColorWidget>> m_widgets;

    MyGuiScreen(EGE::GUIGameLoop& loop)
    : EGE::GUIScreen(loop) {}

    void onLoad()
    {
        layoutDirection = EGE::LayoutElement::Direction::Vertical;
        DEBUG_PRINT("MyGuiScreen onLoad");
        size_t s = 1;
        for(auto& color: MyGameplayObjectManager::instance->colors)
        {
            auto widget = make<ColorWidget>(*this, s, color.get());
            widget->setSize({"0N", "0N"});
            m_widgets.push_back(widget);
            addWidget(widget);
            s++;
        }
    }
};

class MyResourceManager : public EGE::ResourceManager
{
public:
    virtual bool reload() override
    {
        DEBUG_PRINT("MyResourceManager reload");
        setDefaultFont("font.ttf");
        return true;
    }
};

TESTCASE(simple)
{
    auto game = EGE::Game::instance();
    auto gpom = make<MyGameplayObjectManager>();
    game.setGameplayObjectManager(gpom);
    game.getLoop()->openWindow(sf::VideoMode(400, 400), "EGE::Game");
    game.getLoop()->setResourceManager(make<MyResourceManager>());
    game.getLoop()->setCurrentGUIScreen(make<MyGuiScreen>(*game.getLoop()));
    game.getLoop()->setBackgroundColor(sf::Color(127, 127, 127));
    return game.run();
}

RUN_TESTS(game)
