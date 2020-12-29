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

sf::View Scene2D::getView(sf::RenderTarget& target, const sf::View& parentView) const
{
    if(!m_camera.expired())
    {
        sf::View view = parentView;
        CameraObject2D* cam = m_camera.lock().get();
        auto eye = m_camera.lock()->getEyePosition();
        view.setCenter({(float)eye.x, (float)eye.y});
        Vec2d _size;

        switch(cam->getScalingMode())
        {
        case ScalingMode::None:
            {
                _size = getSize();

                // The SFML camera is by default centered, so "uncenter" it.
                view.move({(float)_size.x / 2, (float)_size.y / 2});
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
                    _size = Vec2d(cam->getSize().y * aspect, cam->getSize().y);
                else
                    _size = Vec2d(cam->getSize().x, cam->getSize().x / aspect);
            }
            break;
        case ScalingMode::XLocked:
            {
                float aspect = getSize().x / getSize().y;
                _size = Vec2d(cam->getFOV(), cam->getFOV() / aspect);
            }
            break;
        case ScalingMode::YLocked:
            {
                float aspect = getSize().x / getSize().y;
                _size = Vec2d(cam->getFOV() * aspect, cam->getFOV());
            }
            break;
        case ScalingMode::Scaled:
            {
                _size = cam->getSize();
            }
            break;
        }

        view.setSize({(float)_size.x, (float)_size.y});
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
        view.setCenter({});
        return view;
    }
}

sf::View Scene2D::getCustomView(sf::RenderTarget& target) const
{
    return getView(target, target.getView());
}

Vec2d Scene2D::mapScreenToScene(sf::RenderTarget& target, sf::Vector2i screenPos, const sf::View& parentView)
{
    auto vec = target.mapPixelToCoords(screenPos, getView(target, parentView));
    return {vec.x, vec.y};
}

sf::Vector2i Scene2D::mapSceneToScreen(sf::RenderTarget& target, Vec2d scenePos, const sf::View& parentView)
{
    return target.mapCoordsToPixel({(float)scenePos.x, (float)scenePos.y}, getView(target, parentView));
}

}
