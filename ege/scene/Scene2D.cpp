/*
EGE - Extendable Game Engine
Copyright (c) Sppmacd 2020
*/

#include "Scene2D.h"

#include <ege/debug/Logger.h>

namespace EGE
{

void Scene2D::setCamera(std::weak_ptr<CameraObject2D> camera)
{
    m_camera = camera;
}

sf::View Scene2D::getView(sf::RenderTarget& target, const sf::View& parentView)
{
    if(!m_camera.expired())
    {
        sf::View view = parentView;
        CameraObject2D* cam = m_camera.lock().get();
        view.setCenter(m_camera.lock()->getEyePosition());
        sf::Vector2f _size;

        switch(cam->getScalingMode())
        {
        case ScalingMode::None:
            {
                _size = getSize();

                // The SFML camera is by default centered, so "uncenter" it.
                view.move(getSize() / 2.f);
            }
            break;
        case ScalingMode::Centered:
            {
                _size = getSize();
            }
            break;
        case ScalingMode::Fit:
            {
                float aspect = getSize().x / getSize().y;
                float sizeAspect = cam->getSize().x / cam->getSize().y;
                DUMP(CAMERA_DEBUG, aspect);
                DUMP(CAMERA_DEBUG, sizeAspect);
                if(aspect > sizeAspect)
                    _size = sf::Vector2f(cam->getSize().y * aspect, cam->getSize().y);
                else
                    _size = sf::Vector2f(cam->getSize().x, cam->getSize().x / aspect);
            }
            break;
        case ScalingMode::XLocked:
            {
                float aspect = getSize().x / getSize().y;
                _size = sf::Vector2f(cam->getFOV(), cam->getFOV() / aspect);
            }
            break;
        case ScalingMode::YLocked:
            {
                float aspect = getSize().x / getSize().y;
                _size = sf::Vector2f(cam->getFOV() * aspect, cam->getFOV());
            }
            break;
        case ScalingMode::Scaled:
            {
                _size = cam->getSize();
            }
            break;
        }

        view.setSize(_size);
        view.zoom(1.f / cam->getZoom());
        return view;
    }
    else
    {
        static bool warned = false;
        if(!warned)
        {
            warned = true;
            err(LogLevel::Warning) << "000F EGE/scene: Scene2D: no camera set, defaulting to {[0,0] 0x 0dg} transform.";
        }

        sf::View view = target.getView();
        view.setCenter(sf::Vector2f(0.f, 0.f));
        return view;
    }
}

void Scene2D::renderOnly(sf::RenderTarget& target, const RenderStates& states)
{
    target.setView(getView(target, target.getView()));

    // render objects iteratively
    Scene::renderOnly(target, states);
}

sf::Vector2f Scene2D::mapScreenToScene(sf::RenderTarget& target, sf::Vector2i screenPos, const sf::View& parentView)
{
    return target.mapPixelToCoords(screenPos, getView(target, parentView));
}

sf::Vector2i Scene2D::mapSceneToScreen(sf::RenderTarget& target, sf::Vector2f scenePos, const sf::View& parentView)
{
    return target.mapCoordsToPixel(scenePos, getView(target, parentView));
}

}
