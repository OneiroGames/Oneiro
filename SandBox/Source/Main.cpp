//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Core/Logger.hpp"
#include "Oneiro/Runtime/Application.hpp"
#include "Oneiro/Renderer/OpenGL/Sprite2D.hpp"
#include "HazelAudio/HazelAudio.h"

class SandBoxApp final : public oe::Runtime::Application
{
public:
    bool Init() override
    {
        oe::log::get("log")->info("Initializing...");

        mBackground2D.Init("Assets/Textures/background.jpg", false);
        mSprite2D.Init("Assets/Textures/sprite.png");

        mAudioSource.LoadFromFile("Assets/Audio/music.mp3");

        mAudioSource.SetVolume(0.5f);
        mAudioSource.Play();

        return true;
    }

    bool Update() override
    {
        mBackground2D.Draw();
        mSprite2D.Draw();

        return true;
    }

    void Shutdown() override
    {
        oe::log::get("log")->info("Shutdown...");
    }

    void HandleKey(oe::Input::Key key, oe::Input::Action action) override
    {
        using namespace oe;
        if (action == Input::Action::PRESS)
        {
            switch (key)
            {
            case Input::Key::ESC:
                Stop();
                break;
            default:
                break;
            }
        }
    }


private:
    Hazel::Audio::Source mAudioSource;
    oe::Renderer::Sprite2D mSprite2D;
    oe::Renderer::Sprite2D mBackground2D;
};

namespace oe::Runtime
{
    std::shared_ptr<Application> CreateApplication(int, char* [])
    {
        return std::make_shared<SandBoxApp>();
    }
}