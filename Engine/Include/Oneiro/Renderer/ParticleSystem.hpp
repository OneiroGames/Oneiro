//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

namespace oe::World
{
    class World;
}

namespace oe::Renderer
{
    struct ParticleProps
    {
        glm::vec4 ColorBegin{1.0f}, ColorEnd{0.0f};
        glm::vec2 Position{};
        glm::vec3 Rotation{1.0f};
        glm::vec2 Velocity{}, VelocityVariation{};
        float RotationAngleBegin{}, RotationAngleEnd{};
        float SizeBegin{}, SizeEnd{}, SizeVariation{};
        float LifeTime{1.0f};
    };

    class ParticleSystem
    {
      public:
        ParticleSystem();

      private:
        friend class World::World;

        void Emit(const ParticleProps& particleProps);
        void Update(float deltaTime);
        void Render();

        struct Particle
        {
            glm::vec4 ColorBegin{}, ColorEnd{};
            glm::vec3 Rotation{};
            glm::vec2 Position{};
            glm::vec2 Velocity{};
            float RotationAngle{};
            float SizeBegin{}, SizeEnd{};
            float LifeTime{1.0f};
            float LifeRemaining{};
            bool Active{};
        };

        std::vector<Particle> mParticlePool{};
        uint32_t mPoolIndex{9999};
    };
} // namespace oe::Renderer