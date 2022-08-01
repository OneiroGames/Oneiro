//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/World/Components.hpp"

namespace oe
{
    TagComponent::TagComponent() = default;

    TagComponent::TagComponent(const TagComponent&) = default;

    TagComponent::TagComponent(std::string tag) : Tag(std::move(tag)) {}

    TransformComponent::TransformComponent() = default;
    TransformComponent::TransformComponent(const TransformComponent&) = default;

    TransformComponent::TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
        : Translation(translation), Rotation(rotation), Scale(scale)
    {
    }

    glm::mat4 TransformComponent::GetTransform() const
    {
        const glm::mat4 rotation = toMat4(glm::quat(Rotation));
        return translate(glm::mat4(1.0f), Translation) * rotation * scale(glm::mat4(1.0f), Scale);
    }

    PointComponent::PointComponent() = default;
    LineComponent::LineComponent() = default;
    CircleComponent::CircleComponent() = default;
    QuadComponent::QuadComponent() = default;

    Sprite2DComponent::Sprite2DComponent() : Sprite2D(new Renderer::GL::Sprite2D) {}

    Sprite2DComponent::Sprite2DComponent(const Sprite2DComponent&) = default;

    Sprite2DComponent::Sprite2DComponent(Renderer::GL::Sprite2D* sprite2D) : Sprite2D(sprite2D) {}

    AnimationComponent::AnimationComponent() = default;

    AnimationComponent::AnimationComponent(const AnimationComponent&) = default;

    AnimationComponent::AnimationComponent(Animation::Animation* animation) : Animation(animation) {}

    ModelComponent::ModelComponent() : Model(new Renderer::GL::Model) {}

    ModelComponent::ModelComponent(const ModelComponent&) = default;

    ModelComponent::ModelComponent(Renderer::GL::Model* model) : Model(model) {}

    MainCameraComponent::MainCameraComponent() : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Up(glm::vec3(0.0f, 1.0f, 0.0f)), WorldUp(Up)
    {
        UpdateCameraVectors();
    }

    void MainCameraComponent::UpdateForward(float dt)
    {
        const float velocity = MovementSpeed * dt;
        Position += Front * velocity;
        UpdateCameraVectors();
    }

    void MainCameraComponent::UpdateBackward(float dt)
    {
        const float velocity = MovementSpeed * dt;
        Position -= Front * velocity;
        UpdateCameraVectors();
    }

    void MainCameraComponent::UpdateRight(float dt)
    {
        const float velocity = MovementSpeed * dt;
        Position += Right * velocity;
        UpdateCameraVectors();
    }

    void MainCameraComponent::UpdateLeft(float dt)
    {
        const float velocity = MovementSpeed * dt;
        Position -= Right * velocity;
        UpdateCameraVectors();
    }

    glm::mat4 MainCameraComponent::GetViewMatrix() const
    {
        return lookAt(Position, Position + Front, Up);
    }

    void MainCameraComponent::UpdateMouse(float xPos, float yPos, bool constrainPitch)
    {
        if (mFirstMouse)
        {
            mLastX = xPos;
            mLastY = yPos;
            mFirstMouse = false;
        }

        float xOffset = xPos - mLastX;
        float yOffset = mLastY - yPos;

        mLastX = xPos;
        mLastY = yPos;

        xOffset *= MouseSensitivity;
        yOffset *= MouseSensitivity;

        Yaw += xOffset;
        Pitch += yOffset;

        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        UpdateCameraVectors();
    }

    glm::mat4 MainCameraComponent::GetPerspectiveProjection() const
    {
        return glm::perspective(glm::radians(Fov),
                                static_cast<float>(Core::Root::GetWindow()->GetWidth()) /
                                    static_cast<float>(Core::Root::GetWindow()->GetHeight()),
                                PerspectiveNear, PerspectiveFar);
    }

    glm::mat4 MainCameraComponent::GetOrthoProjection() const
    {
        return glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, OrthoNear, OrthoFar);
    }

    void MainCameraComponent::UpdateCameraVectors()
    {
        Front = normalize(glm::vec3{cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)), sin(glm::radians(Pitch)),
                                    sin(glm::radians(Yaw)) * cos(glm::radians(Pitch))});

        Right = normalize(cross(Front, WorldUp));
        Up = normalize(cross(Right, Front));
    }
} // namespace oe
