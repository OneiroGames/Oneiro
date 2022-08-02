//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "SandBoxApp.hpp"
#include "Oneiro/Renderer/Gui/GuiLayer.hpp"
#include "Oneiro/Renderer/Renderer.hpp"

namespace SandBox
{
    bool Application::OnInit()
    {
        using namespace oe;

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

        mWorld->CreateEntity("Sprite2D").AddComponent<Sprite2DComponent>().Sprite2D->Load("Assets/Images/sprite.png");
        mWorld->CreateEntity("Cube").AddComponent<ModelComponent>().Model->Load("Assets/Models/cube.fbx");
        mWorld->GetEntity("Cube").GetComponent<TransformComponent>().Translation = glm::vec3(2.5f, 0.0f, 2.5f);

        return true;
    }

    bool Application::OnUpdate(float deltaTime)
    {
        using namespace oe;

        const auto playerEntity = mWorld->GetEntity("Player");

        auto& mainCamera = playerEntity.GetComponent<MainCameraComponent>();

        if (IsKey(Input::PRESS, Input::W))
            mainCamera.UpdateForward(deltaTime);
        if (IsKey(Input::PRESS, Input::S))
            mainCamera.UpdateBackward(deltaTime);
        if (IsKey(Input::PRESS, Input::D))
            mainCamera.UpdateRight(deltaTime);
        if (IsKey(Input::PRESS, Input::A))
            mainCamera.UpdateLeft(deltaTime);

        mWorld->UpdateEntities();

        {
            using namespace Renderer;
            if (GuiLayer::Begin("Stats", nullptr,
                                ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoResize))
            {
                const auto& rendererStats = Renderer::GetStats();
                const auto& rendererLimits = Renderer::GetLimits();

                GuiLayer::SetWindowPos(ImVec2(0, 0));

                GuiLayer::Text("Vertices %i", rendererStats.Vertices);
                GuiLayer::Text("Indices %i", rendererStats.Indices);
                GuiLayer::Text("Textures %i", rendererStats.Textures);
                GuiLayer::Text("Draw Calls %i", rendererStats.DrawCalls);
                GuiLayer::Text("Flushes Count %i", rendererStats.FlushesCount);

                GuiLayer::Text("Max Quads %i", rendererLimits.MaxQuads);
                GuiLayer::Text("Max Vertices %i", rendererLimits.MaxVertices);
                GuiLayer::Text("Max Indices %i", rendererLimits.MaxIndices);
                GuiLayer::Text("Max Textures %i", rendererLimits.MaxTextures);

                GuiLayer::End();
            }

            Renderer::ResetStats();
        }

        return true;
    }

    void Application::OnEvent(const oe::Core::Event::Base& e)
    {
        if (typeid(e) == typeid(oe::Core::Event::CursorPosEvent))
        {
            const auto& cursorEvent = dynamic_cast<const oe::Core::Event::CursorPosEvent&>(e);
            auto& mainCamera = mWorld->GetEntity("Player").GetComponent<oe::MainCameraComponent>();
            mainCamera.UpdateMouse(static_cast<float>(cursorEvent.XPos), static_cast<float>(cursorEvent.YPos));
        }
    }

    void Application::OnShutdown()
    {
        mWorld->Save(true);
    }
} // namespace SandBox

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int, char*[])
    {
        return std::make_shared<SandBox::Application>("SandBox", 1280, 720);
    }
} // namespace oe::Runtime