#include <testsuite/Tests.h>
#include <ege/gui.h>

class TestWidget : public EGE::Widget
{
public:
    TestWidget(EGE::Widget& parent)
    : EGE::Widget(parent) {}

    virtual void updateGeometry(EGE::Renderer&) override
    {
        if(!m_font)
        {
            m_font = getLoop().getResourceManager()->getDefaultFont().get();
            ASSERT(m_font);
        }
    }

    virtual void render(EGE::Renderer& renderer) const override
    {
        EGE::Renderer::TextWithBackgroundSettings settings;
        settings.color = EGE::Colors::white;
        settings.font_size = 15;
        settings.padding = 5;
        settings.text_align = EGE::Renderer::TextAlign::Center;
        renderer.renderTextWithBackground(getSize().x / 2, getSize().y / 2, *m_font, L"test 1234 abdegeąəęóą", settings);
    }

private:
    sf::Font* m_font = nullptr;
};

TESTCASE(renderTextWithBackground)
{
    EGE::GUIGameLoop loop;
    auto window = loop.openWindow(sf::VideoMode(500, 500), "renderTextWithBackground");

    auto resourceManager = make<EGE::GUIResourceManager>();
    resourceManager->registerDefaultFont("font.ttf");
    loop.setResourceManager(resourceManager);

    auto guiScreen = window->setNewGUIScreen<EGE::GUIScreen>();
    guiScreen->addNewWidget<TestWidget>();

    return loop.run();
}

RUN_TESTS(gfx)
