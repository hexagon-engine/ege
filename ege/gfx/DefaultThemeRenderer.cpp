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
    renderer.renderRectangle(x + 1, y + 1, width - 2, height - 2, sf::Color(209, 209, 209), sf::Color(255, 255, 255));
    renderer.renderRectangle(x, y, width - 1, height - 1, sf::Color::Transparent, sf::Color(60, 60, 60));
}

void DefaultThemeRenderer::renderTextBoxLikeBackground(Renderer& renderer, double x, double y, double width, double height)
{
    renderer.renderRectangle(x + 2, y + 2, width - 4, height - 4, sf::Color::White, sf::Color(60, 60, 60));
}

void DefaultThemeRenderer::renderTextBoxLikeBorder(Renderer& renderer, double x, double y, double width, double height)
{
    renderer.renderRectangle(x + 1, y + 1, width - 3, height - 3, sf::Color::Transparent, sf::Color(173, 173, 173));
    renderer.renderRectangle(x + 1, y + 1, width - 2, height - 2, sf::Color::Transparent, sf::Color(210, 210, 210));
}

}

