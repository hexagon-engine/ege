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

#include "Shader.h"

#include <ege3d/window/GLError.h>
#include <ege3d/window/GLEW.h>

#include <ege/debug/Logger.h>
#include <ege/main/Config.h>

#include <GL/glew.h>
#include <GL/glext.h>

namespace EGE3d
{

EGE::SharedPtr<ShaderObject> ShaderObject::create(EGE::String const& source, Type type)
{
    return EGE::SharedPtr<ShaderObject>(new ShaderObject(source, type));
}

ShaderObject::ShaderObject(EGE::String const& source, Type type)
: m_type(type)
{
    ASSERT(EGE3d::hasGLExtension(""));

    // TODO: Check if these shader types are supported
    GLenum shaderType;
    switch(type)
    {
        case Vertex:            shaderType = GL_VERTEX_SHADER; break;
        case TessControl:       shaderType = GL_TESS_CONTROL_SHADER; break;
        case TessEvaluation:    shaderType = GL_TESS_EVALUATION_SHADER; break;
        case Geometry:          shaderType = GL_GEOMETRY_SHADER; break;
        case Fragment:          shaderType = GL_FRAGMENT_SHADER; break;
        case Compute:           shaderType = GL_COMPUTE_SHADER; break;
        default: CRASH();
    }
    EGE3D_GLCHECK(m_id = glCreateShader(shaderType));
    char const* sources[] = {
        source.c_str()
    };
    int const lengths[] = {
        static_cast<int>(source.size())
    };
    EGE3D_GLCHECK(glShaderSource(m_id, 1, sources, lengths));
    EGE3D_GLCHECK(glCompileShader(m_id));
    GLint success = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
	    GLint maxLength = 0;
	    EGE3D_GLCHECK(glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength));
        EGE::String errorMessage;
        errorMessage.resize(maxLength);
        EGE3D_GLCHECK(glGetShaderInfoLog(m_id, maxLength, &maxLength, errorMessage.data()));
        ege_log.error() << "ege3d: Failed to compile shader object: " << errorMessage;
        EGE3D_GLCHECK(glDeleteShader(m_id));
        return;
    }

    m_valid = true;
}

ShaderObject::~ShaderObject()
{
    EGE3D_GLCHECK(glDeleteShader(m_id));
}

EGE::SharedPtr<Shader> Shader::create(EGE::SharedPtrVector<ShaderObject> const& shaderObjects)
{
    return EGE::SharedPtr<Shader>(new Shader(shaderObjects));
}

Shader::Shader(EGE::SharedPtrVector<ShaderObject> const& shaderObjects)
{
    EGE3D_GLCHECK(m_id = glCreateProgram());
    for(auto& shaderObject : shaderObjects)
    {
        ASSERT(shaderObject->valid());
        EGE3D_GLCHECK(glAttachShader(m_id, shaderObject->id()));
    }

    // TODO: Attrib locations if supported
    EGE3D_GLCHECK(glLinkProgram(m_id));
    GLint success = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if(!success)
    {
	    GLint maxLength = 0;
	    EGE3D_GLCHECK(glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength));
        EGE::String errorMessage;
        errorMessage.resize(maxLength);
        EGE3D_GLCHECK(glGetProgramInfoLog(m_id, maxLength, &maxLength, errorMessage.data()));
        ege_log.error() << "ege3d: Failed to link shader program: " << errorMessage;
        EGE3D_GLCHECK(glDeleteProgram(m_id));
        return;
    }

    for(auto& shaderObject : shaderObjects)
        EGE3D_GLCHECK(glDetachShader(m_id, shaderObject->id()));

    m_valid = true;
}

Shader::~Shader()
{
    EGE3D_GLCHECK(glDeleteProgram(m_id));
}

void Shader::setUniform(EGE::String const& name, float value)
{
    unsigned location = glGetUniformLocation(m_id, name.c_str());
    EGE3D_GLCHECK();
    EGE3D_GLCHECK(glProgramUniform1f(m_id, location, value));
}

void Shader::bind(Shader const* shader)
{
    if(shader)
    {
        ASSERT(shader->valid());
        EGE3D_GLCHECK(glUseProgram(shader->id()));
    }
}

namespace Shaders
{

EGE::SharedPtr<Shader> createBasic()
{
    static char const* VERTEX_SHADER = R"~~~(
// TODO: Bump version to 330 core when attrib locations will be implemented
#version 110
varying vec4 vColor;
void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vColor = gl_Color;
}
)~~~";
    static char const* FRAGMENT_SHADER = R"~~~(
// TODO: Bump version to 330 core when attrib locations will be implemented
#version 110
varying vec4 vColor;
void main()
{
    gl_FragColor = vColor;
}
)~~~";
    return Shader::create({
        ShaderObject::create(VERTEX_SHADER, ShaderObject::Vertex),
        ShaderObject::create(FRAGMENT_SHADER, ShaderObject::Fragment)
    });
}

}

}
