//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

#include "Base.hpp"
#include "Oneiro/Core/Logger.hpp"

namespace oe::Renderer::GL
{
    enum ShaderType : uint8_t
    {
        VERTEX = 0,
        FRAGMENT
    };

    class Shader
    {
      public:
        Shader() = default;
        Shader(const Shader&) = delete;
        ~Shader();

        template <int ShaderType> constexpr void LoadShaderSrc(const char* src)
        {
            if (mID != 0)
                return;
            const uint32_t shader = gl::CreateShader(ShaderType);
            gl::ShaderSource(shader, 1, &src, nullptr);
            gl::CompileShader(shader);

            if (CheckCompileError(shader, "SHADER"))
                mShaderSources.push_back(shader);
            else
                gl::DeleteShader(shader);
        }

        template <int ShaderType> constexpr void LoadShaderSrc(const std::string& src)
        {
            LoadShaderSrc<ShaderType>(src.c_str());
        }

        void CreateProgram();

        void Use() const;

        void SetUniform(const char* name, unsigned char value) const
        {
            gl::Uniform1i(GetUniformLocation(name), value);
        }

        void SetUniform(const char* name, char value) const
        {
            gl::Uniform1i(GetUniformLocation(name), value);
        }

        void SetUniform(const char* name, unsigned int value) const
        {
            gl::Uniform1ui(GetUniformLocation(name), value);
        }

        void SetUniform(const char* name, unsigned int first, unsigned int second) const
        {
            gl::Uniform2ui(GetUniformLocation(name), first, second);
        }

        void SetUniform(const char* name, unsigned int first, unsigned int second, unsigned int third) const
        {
            gl::Uniform3ui(GetUniformLocation(name), first, second, third);
        }

        void SetUniform(const char* name, unsigned int first, unsigned int second, unsigned int third, unsigned int fourth) const
        {
            gl::Uniform4ui(GetUniformLocation(name), first, second, third, fourth);
        }

        void SetUniform(const char* name, int value) const
        {
            gl::Uniform1i(GetUniformLocation(name), value);
        }

        void SetUniform(const char* name, int first, int second) const
        {
            gl::Uniform2i(GetUniformLocation(name), first, second);
        }

        void SetUniform(const char* name, int first, int second, int third) const
        {
            gl::Uniform3i(GetUniformLocation(name), first, second, third);
        }

        void SetUniform(const char* name, int first, int second, int third, int fourth) const
        {
            gl::Uniform4i(GetUniformLocation(name), first, second, third, fourth);
        }

        void SetUniform(const char* name, float value) const
        {
            gl::Uniform1f(GetUniformLocation(name), value);
        }

        void SetUniform(const char* name, float first, float second) const
        {
            gl::Uniform2f(GetUniformLocation(name), first, second);
        }

        void SetUniform(const char* name, float first, float second, float third) const
        {
            gl::Uniform3f(GetUniformLocation(name), first, second, third);
        }

        void SetUniform(const char* name, float first, float second, float third, float fourth) const
        {
            gl::Uniform4f(GetUniformLocation(name), first, second, third, fourth);
        }

        void SetUniform(const char* name, const glm::vec2& vec) const
        {
            gl::Uniform2fv(GetUniformLocation(name), 1, &vec[0]);
        }

        void SetUniform(const char* name, const glm::vec3& vec) const
        {
            gl::Uniform3fv(GetUniformLocation(name), 1, &vec[0]);
        }

        void SetUniform(const char* name, const glm::vec4& vec) const
        {
            gl::Uniform4fv(GetUniformLocation(name), 1, &vec[0]);
        }

        void SetUniform(const char* name, const glm::mat4& mat) const
        {
            gl::UniformMatrix4fv(GetUniformLocation(name), 1, gl::FALSE_, &mat[0][0]);
        }

        [[maybe_unused]] void ClearUniformLocationCache();

        bool LoadShaderFile(const char* path);

        uint32_t Get();

      private:
        static bool CheckCompileStatus(uint32_t id);

        static bool CheckCompileError(uint32_t id, const char* type);

        GLint GetUniformLocation(const std::string& name) const
        {
            if (mUniformLocationCache.contains(name))
                return mUniformLocationCache[name];
            const GLint location = gl::GetUniformLocation(mID, name.c_str());
            mUniformLocationCache[name] = location;
            return location;
        }

        mutable std::unordered_map<std::string, GLint> mUniformLocationCache;
        std::vector<uint32_t> mShaderSources{};
        uint32_t mID{};
    };
} // namespace oe::Renderer::GL
