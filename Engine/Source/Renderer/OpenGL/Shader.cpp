//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Shader.hpp"
#include <cstring>

namespace oe::Renderer::GL
{
    Shader::~Shader()
    {
        if (mID)
            gl::DeleteProgram(mID);
    }

    void Shader::CreateProgram()
    {
        mID = gl::CreateProgram();
        for (auto shaderSource : mShaderSources)
            gl::AttachShader(mID, shaderSource);
        gl::LinkProgram(mID);
        CheckCompileError(mID, "PROGRAM");
        for (auto shaderSource : mShaderSources)
            gl::DeleteShader(shaderSource);
    }

    void Shader::Use() const
    {
        gl::UseProgram(mID);
    }

    void Shader::ClearUniformLocationCache()
    {
        mUniformLocationCache.clear();
    }

    uint32_t Shader::Get()
    {
        return mID;
    }

    bool Shader::CheckCompileStatus(uint32_t id)
    {
        int status{};
        gl::GetShaderiv(id, gl::COMPILE_STATUS, &status);
        return status;
    }

    bool Shader::CheckCompileError(uint32_t id, const char* type)
    {
        int success{};
        char infoLog[512];

        if (strcmp(type, "PROGRAM") != 0)
        {
            gl::GetShaderiv(id, gl::COMPILE_STATUS, &success);
            if (!success)
            {
                gl::GetShaderInfoLog(id, 512, nullptr, infoLog);
                std::cerr << infoLog << '\n';
            }
        }
        else
        {
            gl::GetProgramiv(id, gl::LINK_STATUS, &success);
            if (!success)
            {
                gl::GetProgramInfoLog(id, 512, nullptr, infoLog);
                std::cerr << infoLog << '\n';
            }
        }
        return true;
    }
} // namespace oe::Renderer::GL
