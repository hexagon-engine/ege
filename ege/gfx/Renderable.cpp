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

void Renderable::applyStates(Renderer& renderer)
{
    auto& target = renderer.getTarget();
    if(isCustomViewNeeded())
        target.setView(getCustomView(target));
    if(isCustomTransformNeeded())
    {
        auto states = renderer.getStates();
        states.sfStates().transform = getCustomTransform(target);
        renderer.setStates(states);
    }
}

void Renderable::applyStates(Renderer& renderer, RenderStates& states)
{
    auto& target = renderer.getTarget();
    if(isCustomViewNeeded())
        target.setView(getCustomView(target));
    if(isCustomTransformNeeded())
        states.sfStates().transform = getCustomTransform(target);
}

void Renderable::doRender(Renderer& renderer, const RenderStates& states)
{
    auto& target = renderer.getTarget();
    doUpdateGeometry(renderer);
    sf::View oldView = target.getView();
    auto newStates = states;
    applyStates(renderer, newStates);
    renderWithStates(renderer, newStates);
    target.setView(oldView);
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
