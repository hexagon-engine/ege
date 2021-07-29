#include "ege3d/window/GLError.h"
#include <ege3d/window/RenderingState.h>
#include <ege3d/window/SystemEvent.h>
#include <ege3d/window/Renderable.h>
#include <ege3d/window/Renderer.h>
#include <ege/debug/Logger.h>

#include <GL/gl.h>

class MyRenderable : public EGE3d::Renderable
{
public:
    virtual void render(EGE3d::RenderingState const& state) override
    {
        EGE3d::Renderer renderer(state);
        renderer.renderRectangle({10, 10, 20, 20}, EGE::Colors::red);
        renderer.renderCircle({45, 20}, 10, EGE::Colors::red);
        renderer.renderCircle({70, 20}, 10, EGE::Colors::red, 5);
        renderer.renderTexturedRectangle({90, 10, 16, 16}, texture, {0, 0, 32, 32});
        renderer.renderTexturedRectangle({120, 10, 32, 32}, texture, {0, 0, 32, 32});
    }

    virtual void updateGeometry() override
    {
        image = std::make_unique<EGE::ColorRGBA[]>(32*32);
        for(unsigned x = 0; x < 32; x++)
        {
            for(unsigned y = 0; y < 32; y++)
            {
                image[y * 32 + x] = (x+y) % 2 == 0 ? EGE::Colors::green : EGE::Colors::red;
            }
        }
        texture = EGE3d::Texture::createFromColorArray({32, 32}, image.get());
    }

private:
    EGE3d::Texture texture;

    // TODO: Add some EGE3d::Image wrapper for this!!
    EGE::UniquePtr<EGE::ColorRGBA[]> image;
};

int main()
{
    EGE3d::Window window;
    window.create(500, 500, "EGE3d Test", EGE3d::WindowSettings());

    glClearColor(0.0, 0.1, 0.0, 0.0);
    MyRenderable renderable;

    while(window.isOpen())
    {
        while(true)
        {
            auto event = window.nextEvent(false);
            if(!event.hasValue())
                break;
            if(event.value().getEventType() == EGE3d::SystemEventType::EClose)
            {
                window.close();
                break;
            }
        }

        // Clear the window
        // TODO: Maybe this should also go to states?
        glClear(GL_COLOR_BUFFER_BIT);

        // Ortho test.
        EGE3d::RenderingState state(window);
        state.applyClip({500, 500}); // No clip
        renderable.fullRender(state);

        // Flush the back buffer
        window.display();
    }

    window.close();
    return 0;
}
