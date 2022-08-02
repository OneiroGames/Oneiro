//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace oe::Core
{
    class Window;
}

namespace oe::Runtime
{
    class Application;
    class Engine;
} // namespace oe::Runtime

namespace oe::World
{
    class World;
}

namespace oe::Core
{
    class Root
    {
      public: // Getters
        static Window* GetWindow();
        static Runtime::Application* GetApplication();
        static World::World* GetWorld();

      private:
        friend class Runtime::Engine;

      private:
        static Window* mWindowInstance;
        static Runtime::Application* mApplicationInstance;
        static std::shared_ptr<World::World> mWorldInstance;
    };
} // namespace oe::Core
