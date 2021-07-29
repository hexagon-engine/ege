/*
    EGE3d - 3D rendering engine for Hexagon

    Copyright (c) 2021 Hexagon Engine

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once

#include <ege/util/Color.h>
#include <ege/util/Optional.h>
#include <ege/util/Vector.h>

namespace EGE3d
{

class Texture
{
public:
    Texture();
    ~Texture();
    Texture(const Texture& other) = delete;
    Texture& operator=(const Texture& other) = delete;

    Texture(Texture&& other)
    {
        *this = std::move(other);
    }

    Texture& operator=(Texture&& other)
    {
        if(this == &other)
            return *this;
        m_id = std::exchange(other.m_id, 0);
        m_size = std::exchange(other.m_size, {});
        m_initialized = std::exchange(other.m_initialized, {});
        return *this;
    }

    // TODO: createFromImage
    // TODO: Add some Image/Bitmap class
    static Texture createFromColorArray(EGE::Vec2u size, EGE::ColorRGBA* array);
    static Texture createEmpty(EGE::Vec2u size);

    void update(EGE::Vec2u dstPos, EGE::Vec2u srcSize, EGE::ColorRGBA* array);

    unsigned id() const { return m_id; }
    EGE::Vec2u size() const { return m_size; }

    static void bind(Texture const* texture);

private:
    unsigned m_id { 0 };
    EGE::Vec2u m_size;
    bool m_initialized = false;
};

class TextureBinder
{
public:
    TextureBinder(Texture const& texture) { Texture::bind(&texture); }
    ~TextureBinder() { Texture::bind(nullptr); }
};

}
