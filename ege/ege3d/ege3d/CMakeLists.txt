set(SOURCES
    # Window
    "window/GLEW.cpp"
    "window/GLEW.h"
    "window/GLError.cpp"
    "window/GLError.h"
    "window/Keyboard.cpp"
    "window/Keyboard.h"
    "window/Mouse.cpp"
    "window/Mouse.h"
    "window/Renderable.cpp"
    "window/Renderable.h"
    "window/Renderer.cpp"
    "window/Renderer.h"
    "window/RenderingState.cpp"
    "window/RenderingState.h"
    "window/WindowSettings.h"
    "window/Shader.cpp"
    "window/Shader.h"
    "window/SystemEvent.cpp"
    "window/SystemEvent.h"
    "window/Texture.cpp"
    "window/Texture.h"
    "window/Window.cpp"
    "window/Window.h"
    "window/WindowImpl.cpp"
    "window/WindowImpl.h"
    "window/WindowSettings.cpp"
    "window/WindowSettings.h"
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(SOURCES
        ${SOURCES}

        # Window
        "window/XWindowImpl.cpp"
        "window/XWindowImpl.h"
    )
endif()

add_library(ege3d ${SOURCES})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(ege3d
        PRIVATE X11 GL GLU GLEW
    )
else()
    message(FATAL "EGE3d: Target not supported!")
endif()

target_link_libraries(ege3d
    PUBLIC ege-core
)

target_include_directories(ege3d PUBLIC ${CMAKE_SOURCE_DIR})
target_include_directories(ege3d PUBLIC ${CMAKE_SOURCE_DIR}/ege/ege3d)

install(TARGETS "ege3d" ARCHIVE DESTINATION modules)

