/*
    EGE3d - 3D rendering engine for Hexagon

    Copyright (c) 2020 Hexagon Engine

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

#include <ege/util/Types.h>

namespace EGE3d
{

class ShaderObject
{
public:
    enum Type
    {
        Vertex,
        TessControl,
        TessEvaluation,
        Geometry,
        Fragment,
        Compute
    };

    static EGE::SharedPtr<ShaderObject> create(EGE::String const& source, Type type);

    ShaderObject(ShaderObject const&) = delete;
    ShaderObject& operator=(ShaderObject const&) = delete;
    ~ShaderObject();

    Type type() const { return m_type; }
    bool valid() const { return m_valid; }
    unsigned id() const { return m_id; }

private:
    ShaderObject(EGE::String const& source, Type type);
    Type m_type;
    bool m_valid = false;
    unsigned m_id = 0;
};

class Shader
{
public:
    static EGE::SharedPtr<Shader> create(EGE::SharedPtrVector<ShaderObject> const& shaderObjects);
    ~Shader();

    static void bind(Shader const* shader);

    bool valid() const { return m_valid; }
    unsigned id() const { return m_id; }

    void setUniform(EGE::String const& name, float value);

private:
    Shader(EGE::SharedPtrVector<ShaderObject> const& shaderObjects);
    bool m_valid = false;
    unsigned m_id = 0;
};

class ShaderBinder
{
public:
    ShaderBinder(Shader const& shader) { Shader::bind(&shader); }
    ~ShaderBinder() { Shader::bind(nullptr); }
};

namespace Shaders
{

EGE::SharedPtr<Shader> createBasic();

}

}
