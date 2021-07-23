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
    applyFilter(renderer, states);
    renderer.setStates(states);
}

void Renderable::applyStates(Renderer& renderer, RenderStates& states)
{
    auto& target = renderer.getTarget();
    if(isCustomViewNeeded())
        target.setView(getCustomView(target));
    if(isCustomTransformNeeded())
        states.sfStates().transform = getCustomTransform(target);
    applyFilter(renderer, states);
}

void Renderable::applyFilter(Renderer& renderer, RenderStates& states)
{
    // Recursive call check
    if(m_inFilter || !m_filter)
        return;
    m_inFilter = true;
    m_filter->apply(*this, renderer, states);
    m_inFilter = false;
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
