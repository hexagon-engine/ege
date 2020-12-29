/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#pragma once

#include "RenderStates.h"
#include "Renderer.h"

#include <ege/util/Vector.h>
#include <SFML/Graphics.hpp>

namespace EGE
{

class Renderable
{
public:
    void renderWithStates(Renderer& renderer, const RenderStates& states) const;

    // Render with setting view and states.
    virtual void doRender(Renderer& renderer, const RenderStates& states = {});

protected:
    virtual void render(Renderer& renderer) const = 0;

    void setGeometryNeedUpdate(bool val = true) { m_geometryNeedUpdate = val; }

    bool geometryNeedUpdate() const { return m_geometryNeedUpdate; }
    virtual void updateGeometry(Renderer&) {}

    virtual bool isCustomViewNeeded() const { return false; }
    virtual sf::View getCustomView(sf::RenderTarget& target) const { return target.getDefaultView(); }

    virtual void setCustomView(sf::RenderTarget& target);
    virtual void doUpdateGeometry(Renderer& renderer);

private:
    bool m_geometryNeedUpdate = true;
};

}
