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

#include "DefaultThemeRenderer.h"
#include "Renderer.h"

namespace EGE
{

void DefaultThemeRenderer::renderButtonLike(Renderer& renderer, double x, double y, double width, double height)
{
    renderer.renderRectangle(x + 1, y + 1, width - 2, height - 2, ColorRGBA::fromBytes(209, 209, 209), Colors::white);
    renderer.renderRectangle(x, y, width - 1, height - 1, Colors::transparent, ColorRGBA::fromBytes(60, 60, 60));
}

void DefaultThemeRenderer::renderTextBoxLikeBackground(Renderer& renderer, double x, double y, double width, double height)
{
    renderer.renderRectangle(x + 2, y + 2, width - 4, height - 4, Colors::white);
}

void DefaultThemeRenderer::renderTextBoxLikeBorder(Renderer& renderer, double x, double y, double width, double height)
{
    renderer.renderRectangle(x + 2, y + 2, width - 4, height - 4, Colors::transparent, ColorRGBA::fromBytes(60, 60, 60));
    renderer.renderRectangle(x + 1, y + 1, width - 3, height - 3, Colors::transparent, ColorRGBA::fromBytes(173, 173, 173));
    renderer.renderRectangle(x + 1, y + 1, width - 2, height - 2, Colors::transparent, ColorRGBA::fromBytes(210, 210, 210));
}

void DefaultThemeRenderer::renderProgressBar(Renderer& renderer, double x, double y, double width, double height, Progress& progress)
{
    // Background 1 (no progress)
    renderTextBoxLikeBackground(renderer, x, y, width, height);

    // Foreground with progress
    renderer.renderRectangle(x + 2, y + 2, width * progress.getFactor() - 4, height - 4, ColorRGBA::fromBytes(0, 200, 0));

    // Border
    renderTextBoxLikeBorder(renderer, x, y, width, height);
}

}
