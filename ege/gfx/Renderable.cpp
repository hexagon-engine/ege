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
    auto states = renderer.getStates();
    if(isCustomTransformNeeded() || states.sfStates().transform != sf::Transform())
        states.sfStates().transform = getCustomTransform(target);
    if(m_shader)
        states.sfStates().shader = m_shader.get();
    renderer.setStates(states);
}

void Renderable::applyStates(Renderer& renderer, RenderStates& states)
{
    auto& target = renderer.getTarget();
    if(isCustomViewNeeded())
        target.setView(getCustomView(target));
    if(isCustomTransformNeeded())
        states.sfStates().transform = getCustomTransform(target);
    if(m_shader)
        states.sfStates().shader = m_shader.get();
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
