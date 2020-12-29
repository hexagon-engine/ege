/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "RenderStates.h"

#include <ege/util/Vector.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class Renderable
{
public:
    virtual void render(Renderer& renderer) = 0;
    void renderWithStates(Renderer& renderer, const RenderStates& states);

    // Render with setting view and states.
    void doRender(Renderer& renderer, const RenderStates& states);

    virtual bool isCustomViewNeeded() { return false; }
    virtual sf::View getCustomView(sf::RenderTarget& target) { return target.getDefaultView(); }

    bool geometryNeedUpdate() { return m_geometryNeedUpdate; }
    virtual void updateGeometry() {}

protected:
    void setGeometryNeedUpdate(bool val = true) { m_geometryNeedUpdate = val; }

private:
    bool m_geometryNeedUpdate = true;
};

}
