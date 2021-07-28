#include "ege3d/window/RenderingState.h"
#include <ege3d/window/Renderer.h>
#include <ege3d/window/Renderable.h>
#include <ege3d/window/Window.h>
#include <ege3d/window/WindowSettings.h>
#include <ege/debug/Logger.h>
#include <GL/gl.h>
#include <iostream>

void eventHandler(const EGE3d::SystemEvent& event)
{
    if(event.getEventType() == EGE3d::SystemEventType::EClose)
    {
        event.getWindow().close();
    }
}

class MyRenderableChild : public EGE3d::Renderable
{
public:
    MyRenderableChild(EGE::Vec3d translation)
    : m_translation(translation) {}

    virtual void render(EGE3d::RenderingState const& state) override
    {
        // TODO: Find out a way to avoid this call
        // Maybe just updateGeometry before rendering?
        EGE3d::Renderable::updateGeometryIfNeeded();

        EGE3d::Renderer renderer(state); // Flush the states.
        
        // Translation matrix + state propagation test.
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0, 0.5, 0);
        glVertex2f(25, 25);
        glVertex2f(0, 25);
        glVertex2f(25, 50);
        glVertex2f(50, 25);
        glVertex2f(25, 0);
        glVertex2f(0, 25);
        glEnd();
    }

    virtual void updateGeometry() override
    {
        ege_log.info() << "updateGeometry(" << this << ") (this should be displayed once for each address)";
    }

    EGE::Vec3d translation() const { return m_translation; }

private:
    EGE::Vec3d m_translation;
};

class MyRenderable : public EGE3d::Renderable
{
public:
    virtual void render(EGE3d::RenderingState const& state) override
    {
        EGE3d::Renderer renderer(state); // Flush the states.

        // Draw something.
        glBegin(GL_TRIANGLE_STRIP);
        glColor3f(1, 0, 0);
        glVertex2f(0, 0);
        glColor3f(0, 1, 0);
        glVertex2f(100, 0);
        glColor3f(0, 0, 1);
        glVertex2f(0, 100);
        glColor3f(1, 1, 1);
        glVertex2f(100, 100);
        glEnd();

        // State propagation test.
        for(auto it: { &child1, &child2 })
        {
            auto newState = state;
            newState.applyTranslation(it->translation());
            it->render(newState);
        }
    }

    virtual void updateGeometry() override
    {
        for(auto it: { &child1, &child2 })
        {
            it->updateGeometryIfNeeded();
        }
    }

    MyRenderableChild child1 {{50, 50}};
    MyRenderableChild child2 {{10, 10}};
};

int main()
{
    EGE3d::Window window;
    window.create(300, 300, "EGE3d Test", EGE3d::WindowSettings());

    glClearColor(0.0, 0.1, 0.0, 0.0);
    MyRenderable renderable;

    while(window.isOpen())
    {
        while(true)
        {
            auto event = window.nextEvent(false);
            if(!event.hasValue())
                break;
            eventHandler(event.value());
        }

        // Clear the window
        // TODO: Maybe this should also go to states?
        glClear(GL_COLOR_BUFFER_BIT);

        // Ortho test.
        EGE3d::RenderingState state(window);
        state.applyOrtho(0, 100, 100, 0, -1, 1);
        state.setViewport({50, 50, 50, 50});
        renderable.fullRender(state);

        // Flush the back buffer
        window.display();
    }

    window.close();
    return 0;
}
