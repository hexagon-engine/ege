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
    glClearColor(0.0, 1.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 300, 300, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);

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
        glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(300, 300);
        glEnd();

        window.display();
    }

    window.close();
    return 0;
}
