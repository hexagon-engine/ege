#include "Renderable.h"

namespace EGE
{

void Renderable::renderWithStates(Renderer& renderer, const RenderStates& states)
{
    const RenderStates& oldStates = states;
    renderer.setStates(states);
    render(renderer);
    renderer.setStates(oldStates);
}

void Renderable::doRender(Renderer& renderer, const RenderStates& states)
{
    auto target = renderer.getTarget();
    if(isCustomViewNeeded())
        target.setView(getCustomView(target));
    if(geometryNeedUpdate())
    {
        updateGeometry();
        m_geometryNeedUpdate = false;
    }
    renderWithStates(renderer, states);
}

}
