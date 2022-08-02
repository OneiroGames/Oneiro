//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/OpenGL/Shader.hpp"
#include "Oneiro/Core/Logger.hpp"
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
    bool Shader::LoadShaderFile(const char* path)
    {
        std::ifstream file{path};
        if (!file.is_open())
        {
            log::get("log")->warn("Failed to load shader '" + std::string(path) + "'!");
            return false;
        }

        std::stringstream ss[2];
        std::string line{};

        ShaderType type;
        while (std::getline(file, line))
        {
            if (line.find("// TYPE=") != std::string::npos)
            {
                if (line.find("VERTEX") != std::string::npos)
                    type = VERTEX;
                else if (line.find("FRAGMENT") != std::string::npos)
                    type = FRAGMENT;
            }
            else
                ss[(int)type] << line << '\n';
        }

        file.close();

        LoadShaderSrc<gl::VERTEX_SHADER>(ss[(int)VERTEX].str());
        LoadShaderSrc<gl::FRAGMENT_SHADER>(ss[(int)FRAGMENT].str());

        return true;
    }
} // namespace oe::Renderer::GL
