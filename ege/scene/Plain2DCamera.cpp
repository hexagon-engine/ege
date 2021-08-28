/*
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*      ,----  ,----  ,----
*      |      |      |
*      |----  | --,  |----
*      |      |   |  |
*      '----  '---'  '----
*
*     Framework Library for Hexagon
*
*    Copyright (c) Sppmacd 2020 - 2021
*
*    Permission is hereby granted, free of charge, to any person obtaining a copy
*    of this software and associated documentation files (the "Software"), to deal
*    in the Software without restriction, including without limitation the rights
*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*    copies of the Software, and to permit persons to whom the Software is
*    furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in all
*    copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*    SOFTWARE.
*
*   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*
*/

#include "Plain2DCamera.h"
#include "Scene.h"

#include <SFML/Graphics.hpp>

namespace EGE
{

sf::View Plain2DCamera::getView(sf::View parentView) const
{
    sf::View view = parentView;
    auto eye = getEyePosition();
    view.setCenter({(float)eye.x, (float)eye.y});
    Vec2d _size;

    Vec2d parentSize = {parentView.getSize().x, parentView.getSize().y};

    switch(m_scalingMode)
    {
    case ScalingMode::None:
        {
            _size = parentSize;

            // The SFML camera is by default centered, so "uncenter" it.
            view.move({(float)_size.x / 2, (float)_size.y / 2});
        }
        break;
    case ScalingMode::Centered:
        {
            _size = parentSize;
        }
        break;
    case ScalingMode::Fit:
        {
            float aspect = parentView.getSize().x / parentView.getSize().y;
            float sizeAspect = getDisplaySize().x / getDisplaySize().y;
            if(aspect > sizeAspect)
                _size = Vec2d(getDisplaySize().y * aspect, getDisplaySize().y);
            else
                _size = Vec2d(getDisplaySize().x, getDisplaySize().x / aspect);
        }
        break;
    case ScalingMode::XLocked:
        {
            float aspect = parentView.getSize().x / parentView.getSize().y;
            _size = Vec2d(getFOV(), getFOV() / aspect);
        }
        break;
    case ScalingMode::YLocked:
        {
            float aspect = parentView.getSize().x / parentView.getSize().y;
            _size = Vec2d(getFOV() * aspect, getFOV());
        }
        break;
    case ScalingMode::Scaled:
        {
            _size = getDisplaySize();
        }
        break;
    }

    view.setSize({(float)_size.x, (float)_size.y});
    view.zoom(1.f / getZoom());
    return view;
}

void Plain2DCamera::applyTransform(Renderer& renderer) const
{
    // TODO: Support parent views properly
    renderer.getTarget().setView(getView(renderer.getTarget().getView()));
}

Vec2d Plain2DCamera::mapToScreenCoords(Renderer& renderer, Vec3d scene) const
{
    // FIXME: Narrowing
    // TODO: Support parent views properly
    auto pixel = renderer.getTarget().mapCoordsToPixel({static_cast<float>(scene.x), static_cast<float>(scene.y)}, getView(renderer.getTarget().getView()));
    return {static_cast<double>(pixel.x), static_cast<double>(pixel.y)};
}

Vec3d Plain2DCamera::mapToSceneCoords(Renderer& renderer, Vec2d screen) const
{
    // FIXME: Narrowing
    // TODO: Support parent views properly
    auto scene = renderer.getTarget().mapPixelToCoords({static_cast<int>(screen.x), static_cast<int>(screen.y)}, getView(renderer.getTarget().getView()));
    return {static_cast<double>(scene.x), static_cast<double>(scene.y)};
}

}
