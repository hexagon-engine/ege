#include <ege3d/window/Renderer.h>
#include <ege3d/window/Window.h>
#include <ege3d/window/WindowSettings.h>
#include <GL/gl.h>
#include <iostream>

void eventHandler(const EGE3d::SystemEvent& event)
{
    if(event.getEventType() == EGE3d::SystemEventType::EClose)
    {
        event.getWindow().close();
    }
}

int main()
{
    EGE3d::Window window;
    window.create(300, 300, "EGE3d Test", EGE3d::WindowSettings());

    // do some OpenGL stuff
    EGE3d::Renderer renderer(window);
    glClearColor(0.0, 0.1, 0.0, 0.0);
    renderer.setMatrixMode(EGE3d::Renderer::MatrixMode::Projection);
    renderer.setMatrixToIdentity();
    glOrtho(0, 100, 100, 0, 0, 1);
    renderer.setMatrixMode(EGE3d::Renderer::MatrixMode::Modelview);
    renderer.setViewport({50, 50, 50, 50});

    while(window.isOpen())
    {
        while(true)
        {
            auto event = window.nextEvent(false);
            if(!event.hasValue())
                break;
            eventHandler(event.value());
        }

        // OPENGL test
        glClear(GL_COLOR_BUFFER_BIT);

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

        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0, 0.5, 0);
        glVertex2f(75, 75);
        glVertex2f(50, 75);
        glVertex2f(75, 100);
        glVertex2f(100, 75);
        glVertex2f(75, 50);
        glVertex2f(50, 75);
        glEnd();

        window.display();
    }

    window.close();
    return 0;
}
