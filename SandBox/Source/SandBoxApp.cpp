//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "SandBoxApp.hpp"

namespace SandBox
{
    bool Application::OnInit()
    {
        using namespace oe;

        constexpr auto vertexShaderSrc = R"(
                #version 330 core
                layout (location = 0) in vec4 aColor;
				layout (location = 1) in vec3 aPos;
				layout (location = 2) in vec2 aNormal;
				layout (location = 3) in vec2 aTexCoords;
                uniform mat4 uView;
                uniform mat4 uProjection;
                uniform mat4 uModel;
				out vec4 Color;
				out vec2 TexCoords;
                void main()
                {
                    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
					TexCoords = aTexCoords;
					Color = aColor;
                }
            )";

        constexpr auto fragmentShaderSrc = R"(
                #version 330 core
                out vec4 FragColor;
				uniform sampler2D uTexture;
				in vec4 Color;
				in vec2 TexCoords;
                void main()
                {
					if (TexCoords.x == 0.0 && TexCoords.y == 0.0)
						FragColor = Color;
					else
						FragColor = pow(texture(uTexture, TexCoords), vec4(1.0/2.2));
                }
            )";

        mShader.LoadShaderSrc<gl::VERTEX_SHADER>(vertexShaderSrc);
        mShader.LoadShaderSrc<gl::FRAGMENT_SHADER>(fragmentShaderSrc);
        mShader.CreateProgram();

        SetMode(Input::CURSOR, Input::CURSOR_DISABLED);

        gl::CullFace(gl::BACK);
        gl::FrontFace(gl::CCW);

        if (World::World::IsExists("main"))
        {
            mWorld = World::World::Load("main");
            return mWorld.get();
        }

        mWorld = std::make_shared<World::World>("Main", "main");
        auto player = mWorld->CreateEntity("Player");
        player.AddComponent<MainCameraComponent>();

        mWorld->CreateEntity("Backpack").AddComponent<ModelComponent>().Model->Load("Assets/Models/backpack/backpack.obj");
        mWorld->CreateEntity("Cube").AddComponent<ModelComponent>().Model->Load("Assets/Models/cube.fbx");

        return true;
    }

    bool Application::OnUpdate(float deltaTime)
    {
        using namespace oe;

        const auto playerEntity = mWorld->GetEntity("Player");
        const auto backpackEntity = mWorld->GetEntity("Backpack");
        const auto cubeEntity = mWorld->GetEntity("Cube");

        auto& mainCamera = playerEntity.GetComponent<MainCameraComponent>();

        auto& backpackTransform = backpackEntity.GetComponent<TransformComponent>();
        auto& playerTransform = playerEntity.GetComponent<TransformComponent>();
        auto& cubeTransform = cubeEntity.GetComponent<TransformComponent>();

        if (IsKey(Input::Action::PRESS, Input::Key::W))
            mainCamera.UpdateForward(deltaTime);
        if (IsKey(Input::Action::PRESS, Input::Key::S))
            mainCamera.UpdateBackward(deltaTime);
        if (IsKey(Input::Action::PRESS, Input::Key::D))
            mainCamera.UpdateRight(deltaTime);
        if (IsKey(Input::Action::PRESS, Input::Key::A))
            mainCamera.UpdateLeft(deltaTime);

        backpackTransform.Rotation += glm::vec3(deltaTime);
        playerTransform.Translation = mainCamera.Position;
        cubeTransform.Translation = glm::vec3(5.5f, 1.0f, 0.5f);
        cubeTransform.Rotation.x = 90.0f;

        mShader.Use();
        mShader.SetUniform("uModel", backpackTransform.GetTransform());
        mShader.SetUniform("uView", mainCamera.GetViewMatrix());
        mShader.SetUniform("uProjection", mainCamera.GetPerspectiveProjection());
        backpackEntity.GetComponent<ModelComponent>().Model->Draw();

        mShader.Use();
        mShader.SetUniform("uModel", cubeTransform.GetTransform());
        mWorld->GetEntity("Cube").GetComponent<ModelComponent>().Model->Draw();

        return true;
    }

    void Application::OnEvent(const oe::Core::Event::Base& e)
    {
        if (typeid(e) == typeid(oe::Core::Event::CursorPosEvent))
        {
            const auto& cursorEvent =
                dynamic_cast<const oe::Core::Event::CursorPosEvent&>(e);
            auto& mainCamera = mWorld->GetEntity("Player").GetComponent<oe::MainCameraComponent>();
            mainCamera.UpdateMouse(static_cast<float>(cursorEvent.XPos), static_cast<float>(cursorEvent.YPos));
        }
    }

    void Application::OnShutdown()
    {
        mWorld->Save(true);
    }
}

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int, char*[])
    {
        return std::make_shared<SandBox::Application>("SandBox", 1280, 720);
    }
}