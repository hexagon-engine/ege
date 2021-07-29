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

#include "Texture.h"

#include "GLError.h"

#include <ege/debug/Logger.h>

#include <GL/gl.h>

namespace EGE3d
{

namespace
{

static bool s_texturesEnabled = false;

}

Texture::Texture()
{
    if(!s_texturesEnabled)
    {
        s_texturesEnabled = true;
        EGE3D_GLCHECK(glEnable(GL_TEXTURE_2D));
    }
}

Texture::~Texture()
{
    ege_log.info() << "Texture::~Texture(" << m_id << ")";
    if(m_id != 0)
        EGE3D_GLCHECK(glDeleteTextures(1, &m_id));
}

Texture Texture::createFromColorArray(EGE::Vec2u size, EGE::ColorRGBA* array)
{
    Texture texture = createEmpty(size);
    texture.update({}, size, array);
    return texture;
}

Texture Texture::createEmpty(EGE::Vec2u size)
{
    Texture texture;
    texture.m_size = size;
    EGE3D_GLCHECK(glGenTextures(1, &texture.m_id));
    return texture;
}

void Texture::update(EGE::Vec2u dstPos, EGE::Vec2u srcSize, EGE::ColorRGBA* array)
{
    ASSERT(dstPos.x + srcSize.x <= m_size.x);
    ASSERT(dstPos.y + srcSize.y <= m_size.y);
    TextureBinder binder(*this);
    if(dstPos == EGE::Vec2u())
    {
        ege_log.info() << "Texture::update dstPos=[0] srcSize=[" << srcSize.x << "," << srcSize.y << "] <- " << array << " id=" << m_id;
        // Disable mipmaps for now
        // FIXME: Code dupe
        EGE3D_GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        EGE3D_GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        EGE3D_GLCHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, array));
        m_initialized = true;
        return;
    }
    else 
    {
        if(!m_initialized)
        {
            // Disable mipmaps for now
            // FIXME: Code dupe
            EGE3D_GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
            EGE3D_GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
            EGE3D_GLCHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
            m_initialized = true;
        }
        ege_log.info() << "Texture::update dstPos=[" << dstPos.x << "," << dstPos.y << "] srcSize=[" << srcSize.x << "," << srcSize.y << "] <- " << array << " id=" << m_id;
        EGE3D_GLCHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, dstPos.x, dstPos.y, srcSize.x, srcSize.y, GL_RGBA8, GL_UNSIGNED_BYTE, array));
    }
}

void Texture::bind(Texture const* texture)
{
    EGE3D_GLCHECK(glBindTexture(GL_TEXTURE_2D, texture ? texture->id() : 0));
}

}
