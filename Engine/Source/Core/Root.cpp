//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Root.hpp"
#include "Oneiro/World/World.hpp"

namespace oe::Core
{
    Window* Root::GetWindow()
    {
        return mWindowInstance;
    }

    Runtime::Application* Root::GetApplication()
    {
        return mApplicationInstance;
    }

    World::World* Root::GetWorld()
    {
        return mWorldInstance.get();
    }

    Window* Root::mWindowInstance{};
    Runtime::Application* Root::mApplicationInstance{};
    std::shared_ptr<World::World> Root::mWorldInstance{};
} // namespace oe::Core
