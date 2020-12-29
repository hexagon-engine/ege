#include "Renderable.h"

namespace EGE
{

void Renderable::renderWithStates(Renderer& renderer, const RenderStates& states) const
{
    const RenderStates& oldStates = states;
    renderer.setStates(states);
    render(renderer);
    renderer.setStates(oldStates);
}

void Renderable::setCustomView(sf::RenderTarget& target)
{
    if(isCustomViewNeeded())
        target.setView(getCustomView(target));
}

void Renderable::doRender(Renderer& renderer, const RenderStates& states)
{
    auto& target = renderer.getTarget();
    setCustomView(target);
    doUpdateGeometry(renderer);
    renderWithStates(renderer, states);
}

void Renderable::doUpdateGeometry(Renderer& renderer)
{
    if(geometryNeedUpdate())
    {
        updateGeometry(renderer);
        m_geometryNeedUpdate = false;
    }
}

}
