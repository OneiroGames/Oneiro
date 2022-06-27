//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/World/Components.hpp"
#include "Oneiro/World/Entity.hpp"

namespace SandBox
{
    class Application final : public oe::Runtime::Application
    {
        using oe::Runtime::Application::Application;
    public:
        bool OnInit() override;
        bool OnUpdate(float deltaTime) override;
        void OnEvent(const oe::Core::Event::Base& e) override;
        void OnShutdown() override;
    private:
        std::shared_ptr<oe::World::World> mWorld{};
        oe::Renderer::GL::Shader mShader{};
    };
}

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int, char*[]);
}
