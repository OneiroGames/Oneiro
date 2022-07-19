//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "OpenGL/gl_core_4_5.hpp"
#include <cstdint>

#define IS_SAME(...) requires(std::is_same<T, __VA_ARGS__>::value)

#define IS_SAME_TEMPLATE(...) template <class T> IS_SAME(__VA_ARGS__)

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace oe::Renderer::GL
{
    struct Vertex
    {
        glm::vec4 Color{};
        glm::vec3 Position{};
        glm::vec3 Normal{};
        glm::vec2 TexCoords{};
    };
} // namespace oe::Renderer::GL
