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

    virtual std::shared_ptr<EGE::ObjectMap> serialize()
    {
        std::shared_ptr<EGE::ObjectMap> map = make<EGE::ObjectMap>();
        map->addObject("r", make<EGE::ObjectInt>(m_color.r));
        map->addObject("g", make<EGE::ObjectInt>(m_color.g));
        map->addObject("b", make<EGE::ObjectInt>(m_color.b));
        map->addObject("a", make<EGE::ObjectInt>(m_color.a));
        return map;
    }

    virtual void deserialize(std::shared_ptr<EGE::ObjectMap> obj)
    {
        m_color.r = obj->getObject("r").lock()->asInt();
        m_color.g = obj->getObject("g").lock()->asInt();
        m_color.b = obj->getObject("b").lock()->asInt();
        m_color.a = obj->getObject("a").lock()->asInt();
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
        return true;
    }
};

MyGameplayObjectManager* MyGameplayObjectManager::instance;

class ColorWidget : public EGE::Widget
{
public:
    MyColor* m_color;
    typedef EGE::GameplayObjectRegistry<std::string, MyColor> GPORObjc;
    GPORObjc::IdTEntry m_id;

    ColorWidget(EGE::Widget* parent, GPORObjc::IdTEntry id, MyColor* color)
    : EGE::Widget(parent)
    , m_color(color)
    , m_id(id)
    {}

    void onResize(sf::Event::SizeEvent& event)
    {
        EGE::Widget::onResize(event);
        m_size = EGE::Vec2d(event.width - 80.f, 32.f);
    }

    void render(sf::RenderTarget& target)
    {
        EGE::Widget::render(target);

        sf::RectangleShape rs(sf::Vector2f(30.f, 30.f));
        rs.setPosition(1.f, 1.f);
        rs.setOutlineColor(sf::Color(255 - m_color->m_color.r, 255 - m_color->m_color.g, 255 - m_color->m_color.b));

        if(m_mouseOver)
            rs.setOutlineThickness(1.f);

        rs.setFillColor(m_color->m_color);
        target.draw(rs);

        EGE::Label label(this);
        label.setString(std::to_string(m_id.numericId) + ": " + m_id.baseId);
        label.setPosition(EGE::Vec2d(40.f, 0.f));
        label.setFontSize(15);
        label.render(target);
    }
};

class MyGuiScreen : public EGE::GUIScreen
{
public:
    std::vector<std::shared_ptr<ColorWidget>> m_widgets;

    MyGuiScreen(EGE::GUIGameLoop* loop)
    : EGE::GUIScreen(loop) {}

    void onLoad()
    {
        DEBUG_PRINT("MyGuiScreen onLoad");
        size_t s = 0;
        for(auto& _color: MyGameplayObjectManager::instance->colors)
        {
            DEBUG_PRINT(_color.first.baseId.c_str());
            DEBUG_PRINT(std::to_string(_color.first.numericId).c_str());
            auto widget = make<ColorWidget>(this, _color.first, _color.second.get());
            widget->setPosition(EGE::Vec2d(40.f, 40.f * s + 40.f));
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
    game.getLoop()->setWindow(make<EGE::SFMLSystemWindow>(sf::VideoMode(400, 400), "EGE::Game"));
    game.getLoop()->setResourceManager(make<MyResourceManager>());
    game.getLoop()->setCurrentGUIScreen(make<MyGuiScreen>(game.getLoop().get()));
    game.getLoop()->setBackgroundColor(sf::Color(127, 127, 127));
    return game.run();
}

RUN_TESTS(game)
