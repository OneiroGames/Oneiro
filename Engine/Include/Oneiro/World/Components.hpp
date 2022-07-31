//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Animation/Animation.hpp"
#include "Oneiro/Renderer/OpenGL/Model.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include "glm/gtx/quaternion.hpp"

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/Core/Window.hpp"
#include "Oneiro/Renderer/OpenGL/Model.hpp"
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"

namespace oe
{
    struct TagComponent
    {
        TagComponent();
        TagComponent(const TagComponent&);

        TagComponent(std::string tag);

        std::string Tag{};
    };

    struct TransformComponent
    {
        TransformComponent();
        TransformComponent(const TransformComponent&);

        TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale);

        glm::vec3 Translation{};
        glm::vec3 Rotation{};
        glm::vec3 Scale{1.0f};

        [[nodiscard]] glm::mat4 GetTransform() const;
    };

    struct Sprite2DComponent
    {
        Sprite2DComponent();
        Sprite2DComponent(const Sprite2DComponent&);

        Sprite2DComponent(Renderer::GL::Sprite2D* sprite2D);

        Renderer::GL::Sprite2D* Sprite2D{};
    };

    struct AnimationComponent
    {
        AnimationComponent();
        AnimationComponent(const AnimationComponent&);

        AnimationComponent(Animation::Animation* animation);

        Animation::Animation* Animation{};
    };

    struct ModelComponent
    {
        ModelComponent();
        ModelComponent(const ModelComponent&);

        ModelComponent(Renderer::GL::Model* model);

        Renderer::GL::Model* Model;
    };

    struct MainCameraComponent
    {
        glm::vec3 Position{}, Front{}, Up{}, WorldUp{}, Right{};

        float Yaw{-90.0f}, Pitch{};

        float MovementSpeed{2.5f}, MouseSensitivity{0.1f};

        float PerspectiveNear{0.1f}, PerspectiveFar{100.0f}, Fov{45.0f};
        float OrthoNear{-1.0f}, OrthoFar{1.0f};

        MainCameraComponent();

        void UpdateForward(float dt);

        void UpdateBackward(float dt);

        void UpdateRight(float dt);

        void UpdateLeft(float dt);

        [[nodiscard]] glm::mat4 GetViewMatrix() const;

        void UpdateMouse(float xPos, float yPos, bool constrainPitch = true);

        [[nodiscard]] glm::mat4 GetPerspectiveProjection() const;

        [[nodiscard]] glm::mat4 GetOrthoProjection() const;

      private:
        void UpdateCameraVectors();
        float mLastX{}, mLastY{};
        bool mFirstMouse{true};
    };
} // namespace oe
