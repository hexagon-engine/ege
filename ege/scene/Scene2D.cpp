/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Scene2D.h"

namespace EGE
{

void Scene2D::setCamera(std::weak_ptr<CameraObject2D> camera)
{
    m_camera = camera;
}

void Scene2D::renderOnly(sf::RenderTarget& target)
{
    // we get view set for this specific widget
    // but we must transform it for camera
    // if camera doesn't exists we use zero coordinates
    if(!m_camera.expired())
    {
        sf::View view = target.getView();
        view.setCenter(m_camera.lock()->getEyePosition());
        target.setView(view);
    }
    else
    {
        sf::View view = target.getView();
        view.setCenter(sf::Vector2f(0.f, 0.f));
        target.setView(view);
        std::cerr << "000F EGE/scene: Scene2D: no camera set, defaulting to {[0,0] 0x 0dg} transform." << std::endl;
    }

    // render objects iteratively
    Scene::renderOnly(target);
}

}
