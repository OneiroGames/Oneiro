//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Renderer/ParticleSystem.hpp"
#include "Oneiro/Core/Random.hpp"
#include "Oneiro/Renderer/Renderer.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/constants.hpp"
#include "glm/gtx/compatibility.hpp"

namespace oe::Renderer
{
    ParticleSystem::ParticleSystem()
    {
        mParticlePool.resize(10000);
    }

    void ParticleSystem::Update(float deltaTime)
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

            particle.LifeRemaining -= deltaTime;
            particle.Position -= particle.Velocity * (float)deltaTime;
        }
    }

    void ParticleSystem::Render()
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

    void ParticleSystem::Emit(const ParticleProps& particleProps)
    {
        Particle& particle = mParticlePool[mPoolIndex];
        particle.ColorBegin = particleProps.ColorBegin;
        particle.ColorEnd = particleProps.ColorEnd;

        particle.Position = particleProps.Position;

        particle.Velocity = particleProps.Velocity;
        particle.Velocity.x += particleProps.VelocityVariation.x * (Core::Random::DiceFloat() - 0.5f);
        particle.Velocity.y += particleProps.VelocityVariation.y * (Core::Random::DiceFloat() - 0.5f);

        particle.Rotation = particleProps.Rotation;
        particle.RotationAngle =
            Core::Random::DiceFloat(particleProps.RotationAngleBegin, particleProps.RotationAngleEnd) * glm::pi<float>();

        particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Core::Random::DiceFloat() - 0.5f);
        particle.SizeEnd = particleProps.SizeEnd;

        particle.LifeTime = particleProps.LifeTime;
        particle.LifeRemaining = particleProps.LifeTime;

        particle.Active = true;

        mPoolIndex = --mPoolIndex % mParticlePool.size();
    }
} // namespace oe::Renderer