//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Renderer.hpp"

#include <vector>

#include "Oneiro/Core/Random.hpp"
#include "glm/gtc/random.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/constants.hpp"
#include "glm/gtx/compatibility.hpp"

namespace oe::Renderer
{
    struct ParticleProps
    {
        glm::vec4 ColorBegin{}, ColorEnd{};
        glm::vec2 Position{};
        glm::vec3 Rotation{};
        glm::vec2 Velocity{}, VelocityVariation{};
        float RotationAngleBegin{}, RotationAngleEnd{};
        float SizeBegin{}, SizeEnd{}, SizeVariation{};
        float LifeTime{1.0f};
    };

    class ParticleSystem
    {
      public:
        ParticleSystem()
        {
            mParticlePool.resize(50000);
        }

        void OnUpdate(float dt)
        {
            for (auto& particle : mParticlePool)
            {
                if (!particle.Active)
                    continue;

                if (particle.LifeRemaining <= 0.0f)
                {
                    particle.Active = false;
                    continue;
                }

                particle.LifeRemaining -= dt;
                particle.Position -= particle.Velocity * (float)dt;
            }
        }
        void OnRender(const oe::MainCameraComponent& mainCamera)
        {
            for (auto& particle : mParticlePool)
            {
                if (!particle.Active)
                    continue;
                const float life = particle.LifeRemaining / particle.LifeTime;
                glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
                color.a = color.a * life;

                const float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

                glm::mat4 transform = glm::translate(glm::mat4(1.0f), {particle.Position.x, particle.Position.y, 0.0f}) *
                                      glm::rotate(glm::mat4(1.0f), glm::radians(particle.RotationAngle), particle.Rotation) *
                                      glm::scale(glm::mat4(1.0f), glm::vec3(size, size, 1.0f));

                Renderer::RenderQuad(transform, color);
            }
        }

        void Emit(const ParticleProps& particleProps)
        {
            Particle& particle = mParticlePool[mPoolIndex];
            particle.ColorBegin = particleProps.ColorBegin;
            particle.ColorEnd = particleProps.ColorEnd;

            particle.Position = particleProps.Position;

            particle.Velocity = particleProps.Velocity;
            particle.Velocity.x += particleProps.VelocityVariation.x * (Core::Random::DiceFloat() - 0.5f);
            particle.Velocity.y += particleProps.VelocityVariation.y * (Core::Random::DiceFloat() - 0.5f);

            particle.Rotation = particleProps.Rotation;
            particle.RotationAngle = Core::Random::DiceFloat() * 90.0f * glm::pi<float>();

            particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Core::Random::DiceFloat() - 0.5f);
            particle.SizeEnd = particleProps.SizeEnd;

            particle.LifeTime = particleProps.LifeTime;
            particle.LifeRemaining = particleProps.LifeTime;

            particle.Active = true;

            mPoolIndex = --mPoolIndex % mParticlePool.size();
        }

      private:
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
        uint32_t mPoolIndex{49999};
    };
} // namespace oe::Renderer