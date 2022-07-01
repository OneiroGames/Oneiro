//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include <filesystem>

#include "Oneiro/Animation/DissolveAnimation.hpp"
#include "Oneiro/Core/Logger.hpp"
//#include "Oneiro/Luau/LuauFile.hpp"
#include "Oneiro/Renderer/Gui/GuiLayer.hpp"
#include "Oneiro/Renderer/OpenGL/Text.hpp"
#include "Oneiro/Runtime/Engine.hpp"
#include "Oneiro/VisualNovel/VNCore.hpp"

namespace
{
    struct TextStruct
    {
        oe::Renderer::GL::Text Text{};
        oe::Renderer::GL::Text Name{};
    };

    TextStruct text;

    std::vector<oe::VisualNovel::Instruction> instructions{};
    oe::Renderer::GL::Sprite2D textBox{};
    oe::Animation::DissolveAnimation textBoxAnim{};
    std::vector<std::string> labels{};

    std::string textToRenderer{};
    std::string textToIterate{};
    size_t currentIt{};
    size_t charItToPush{};
    std::vector<std::pair<oe::Renderer::GL::Sprite2D*, oe::Animation::DissolveAnimation*>> sprite2Ds;
    float textSpeed = 0.05f;

    class FadeMusic
    {
    public:
        FadeMusic(Hazel::Audio::Source* audioSource, bool isReversed = false)
        {
            mAudioSource = audioSource;
            mIsReversed = isReversed;
            mIsStart = true;
        }

        void Update(float dt)
        {
            if (!mIsReversed)
            {
                if (mIsStart)
                    mTime = 0.0f;

                if (mTime >= 1.0f)
                {
                    mIsEnded = true;
                    mTime = 1.0f;
                    return;
                }
                mIsEnded = false;
                mTime += dt / 5;
                mAudioSource->SetVolume(mTime >= 1.0f ? 1.0f : mTime);
            }
            else
            {
                if (mIsStart)
                    mTime = 1.0f;

                if (mTime <= 0.0f)
                {
                    mAudioSource->Stop();
                    mIsEnded = true;
                    mTime = 0.0f;
                    return;
                }

                mIsEnded = false;
                mTime -= dt / 5;
                mTime = mTime < 0.0f ? 0.0f : mTime;
                mAudioSource->SetVolume(mTime);
            }

            if (mIsStart)
                mIsStart = false;
        }

        [[nodiscard]] float GetTime() const
        {
            return mTime;
        }

        [[nodiscard]] bool IsReversed() const
        {
            return mIsReversed;
        }

        [[nodiscard]] bool IsEnded() const
        {
            return mIsEnded;
        }

    private:
        Hazel::Audio::Source* mAudioSource{};
        float mTime{};
        bool mIsReversed{};
        bool mIsStart{};
        bool mIsEnded{};
    };

    std::vector<FadeMusic*> fadeMusicContainer{};
    bool updSprites{};
} // namespace

namespace oe::VisualNovel
{
    void Init(const Lua::File* file)
    {
        text.Text.Init();
        if (!labels.empty())
        {
            JumpToLabel(file, labels[0]);
            textBox.SetAlpha(0.0f);
            textBoxAnim.SetReversed(false);
            NextStep();
        }
    }

    void NextStep()
    {
        if (updSprites)
            return;

        if (textToRenderer.size() < textToIterate.size())
        {
            textToRenderer = textToIterate;
            charItToPush = textToIterate.size();
            return;
        }

        if (currentIt >= instructions.size())
            return;
        auto& instruction = instructions[currentIt];
        switch (instruction.Type)
        {
        case SHOW_SPRITE:
            {
                auto& sprite2D = instruction.Sprite2D;
                sprite2D->Move({});
                sprite2D->SetAlpha(0.0f);
                sprite2Ds.emplace_back(sprite2D, new Animation::DissolveAnimation);
                if (textBox.GetAlpha() >= 1.0f)
                    textBoxAnim.SetReversed(true);
                else
                    textBoxAnim.SetReversed(false);
                currentIt++;
                NextStep();
                break;
            }
        case HIDE_SPRITE:
            {
                auto it = sprite2Ds.begin();
                for (; !(it == sprite2Ds.end()); ++it)
                {
                    if (it->first == instruction.Sprite2D)
                    {
                        it->second->SetReversed(true);
                        textBoxAnim.SetReversed(true);
                        break;
                    }
                }
                currentIt++;
                NextStep();
                break;
            }
        case PLAY_MUSIC:
            {
                instruction.AudioSource->Play();
                instruction.AudioSource->SetVolume(0.0f);
                fadeMusicContainer.emplace_back(new FadeMusic{instruction.AudioSource, false});
                currentIt++;
                NextStep();
                break;
            }
        case STOP_MUSIC:
            {
                fadeMusicContainer.emplace_back(new FadeMusic{instruction.AudioSource, true});
                currentIt++;
                NextStep();
                break;
            }
        case PLAY_SOUND:
            {
                instruction.AudioSource->Play();
                currentIt++;
                NextStep();
                break;
            }
        case STOP_SOUND:
            {
                instruction.AudioSource->Stop();
                currentIt++;
                NextStep();
                break;
            }
        case PLAY_AMBIENT:
            {
                instruction.AudioSource->Play();
                currentIt++;
                NextStep();
                break;
            }
        case STOP_AMBIENT:
            {
                instruction.AudioSource->Stop();
                currentIt++;
                NextStep();
                break;
            }
        case JUMP_TO_LABEL:
            {
                JumpToLabel(instruction.Label.File, instruction.Label.Name);
                NextStep();
                break;
            }
        case MOVE_SPRITE:
            {
                instruction.Sprite2D->Move(instruction.Vector3);
                currentIt++;
                NextStep();
                break;
            }
        case SAY_TEXT:
            {
                textToRenderer.clear();
                charItToPush = 0;
                textToIterate = instruction.Text.Who + ": " + instruction.Text.What;
                currentIt++;
                break;
            }
        case SET_TEXT_SPEED:
            {
                textSpeed = instruction.Vector3[0];
                currentIt++;
                NextStep();
                break;
            }
        }
    }

    void Update(float deltaTime, bool gui)
    {
        // Music
        {
            for (auto& fadeMusic : fadeMusicContainer)
            {
                fadeMusic->Update(Runtime::Engine::GetDeltaTime());

                if (fadeMusic->IsEnded())
                    std::ranges::remove(fadeMusicContainer, fadeMusic);
            }
        }

        // Sprites and text
        {
            static bool drawText{};
            Renderer::GL::Sprite2D* prevSprite2D{};

            for (uint32_t i = 0; i < sprite2Ds.size(); ++i)
            {
                auto* sprite2D = sprite2Ds[i].first;
                auto* animation = sprite2Ds[i].second;

                if (sprite2D->GetAlpha() > 1.0f)
                {
                    sprite2D->SetAlpha(1.0f);
                    animation->SetTime(1.0f);
                }

                if (sprite2Ds[0].first->GetAlpha() <= 1.0f && !sprite2Ds[0].second->IsReversed())
                {
                    sprite2Ds[0].second->Update(sprite2Ds[0].first, Runtime::Engine::GetDeltaTime());
                    updSprites = true;
                }

                static bool reverseBg{};

                if (animation->IsReversed() && !textBoxAnim.IsReversed())
                {
                    if (sprite2D->GetAlpha() >= 0.0f)
                    {
                        animation->Update(sprite2D, Runtime::Engine::GetDeltaTime());
                        updSprites = true;
                        reverseBg = true;
                        if (sprite2D->GetAlpha() <= 0.0f)
                        {
                            sprite2Ds.erase(sprite2Ds.begin() + i);
                            updSprites = true;
                        }
                        prevSprite2D = nullptr;
                    }
                    else if (prevSprite2D && prevSprite2D->GetAlpha() >= 0.0f && !reverseBg)
                    {
                        animation->Update(prevSprite2D, Runtime::Engine::GetDeltaTime());
                        if (sprite2D->GetAlpha() <= 0.0f)
                        {
                            sprite2Ds.erase(sprite2Ds.begin() + i);
                            updSprites = true;
                        }
                        updSprites = true;
                    }

                    drawText = sprite2Ds[0].first->GetAlpha() <= 0.0f && sprite2D->GetAlpha() <= 0.0f;
                }
                else if (!textBoxAnim.IsReversed() && textBox.GetAlpha() <= 0.0f)
                {
                    if (prevSprite2D && sprite2D->GetAlpha() <= 1.0f && prevSprite2D->GetAlpha() >= 1.0f)
                    {
                        animation->Update(sprite2D, Runtime::Engine::GetDeltaTime());
                        updSprites = true;
                    }


                    drawText = sprite2Ds[0].first->GetAlpha() >= 1.0f && sprite2D->GetAlpha() >= 1.0f;
                }
                sprite2D->Draw();
                prevSprite2D = sprite2D;
            }

            static bool canUpdateTextLogic{};
            if (drawText)
            {
                if (textBoxAnim.IsReversed())
                {
                    if (textBox.GetAlpha() >= 0.0f)
                    {
                        textBoxAnim.Update(&textBox, Runtime::Engine::GetDeltaTime());
                        canUpdateTextLogic = false;
                        updSprites = true;
                    }

                    if (textBox.GetAlpha() <= 0.0f)
                    {
                        textBoxAnim.SetReversed(false);
                        canUpdateTextLogic = true;
                        updSprites = false;
                    }
                }
                else
                {
                    if (textBox.GetAlpha() <= 1.0f)
                    {
                        textBoxAnim.Update(&textBox, Runtime::Engine::GetDeltaTime());
                        canUpdateTextLogic = false;
                        updSprites = true;
                    }

                    if (textBox.GetAlpha() >= 1.0f)
                    {
                        canUpdateTextLogic = true;
                        updSprites = false;
                    }
                }

                if (canUpdateTextLogic && textBox.GetAlpha() >= 1.0f)
                {
                    static float time{};
                    time += Runtime::Engine::GetDeltaTime();
                    if (time >= textSpeed && charItToPush < textToIterate.size() && textToRenderer.size() != textToIterate.size())
                    {
                        textToRenderer.push_back(textToIterate[charItToPush]);
                        charItToPush++;
                        time = 0.0f;
                    }
                    text.Text.SetString(textToRenderer);
                }

                textBox.Draw();
                if (textBox.GetAlpha() >= 1.0f)
                    text.Text.Draw({125.0f, 145.0f});
            }
            else
            {
                canUpdateTextLogic = false;
            }
        }

        if (gui)
        {
            using namespace Renderer;

            GuiLayer::Begin("Visual Novel");
            {
                if (GuiLayer::CollapsingHeader("Core"))
                {
                    GuiLayer::Text("Current iterator: %zu", currentIt);
                    if (GuiLayer::CollapsingHeader("Sprite2Ds"))
                    {
                        static size_t selected{};
                        const size_t sprite2DsSize{sprite2Ds.size()};
                        if (ImGui::TreeNode("List"))
                        {
                            for (size_t n = 0; n < sprite2DsSize; n++)
                            {
                                char buf[32];
                                sprintf_s(buf, "Sprite2D %llu", n);
                                if (ImGui::Selectable(buf, selected == n))
                                    selected = n;
                            }
                            ImGui::TreePop();
                        }

                        if (selected < sprite2DsSize)
                        {
                            const auto& sprite = sprite2Ds[selected].first;
                            ImGui::Text("Sprite2D");
                            ImGui::Text("Alpha: %f", sprite->GetAlpha());
                            ImGui::Text("Texture Path: %s", sprite->GetTexture()->GetData()->Path.c_str());
                        }
                    }
                }
            }
            GuiLayer::End();
        }
    }

    std::vector<Instruction>& GetInstructions()
    {
        return instructions;
    }

    std::vector<std::string>& GetLabels()
    {
        return labels;
    }

    void JumpToLabel(const Lua::File* file, const std::string& labelName)
    {
        for (const std::string& label : labels)
        {
            if (label == labelName)
            {
                currentIt = 0;
                instructions.clear();
                /*if (!file->CallFunction("main", label))
                    log::get("log")->warn("Failed to jump to " + labelName + " label!");*/
                return;
            }
        }
    }

    void ShowSprite2D(Renderer::GL::Sprite2D* sprite2D)
    {
        instructions.push_back({SHOW_SPRITE, sprite2D});
    }

    void HideSprite2D(Renderer::GL::Sprite2D* sprite2D)
    {
        instructions.push_back({HIDE_SPRITE, sprite2D});
    }

    void PushInstruction(const Instruction& instruction)
    {
        instructions.push_back(instruction);
    }

    void MoveSprite2D(Renderer::GL::Sprite2D* sprite2D, const glm::vec3& pos)
    {
        instructions.push_back({MOVE_SPRITE, sprite2D, {}, {}, {}, pos});
    }

    void PushInstruction(Instruction&& instruction)
    {
        instructions.push_back(instruction);
    }

    void PushLabel(const std::string& labelName)
    {
        labels.push_back(labelName);
    }

    void SetTextSpeed(float speed)
    {
        instructions.push_back({
            SET_TEXT_SPEED,
            {},
            {},
            {},
            {},
            {speed, {}, {}},
        });
    }

    void LoadTextBox(const std::string& path)
    {
        textBox.Load(path);
        textBox.SetUsingTextureAlpha(true);
        textBox.SetAlpha(0.0f);
    }

    void PlayMusic(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({PLAY_MUSIC, {}, {}, audioSource});
    }

    void StopMusic(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({STOP_MUSIC, {}, {}, audioSource});
    }

    void PlaySound(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({PLAY_SOUND, {}, {}, audioSource});
    }

    void StopSound(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({STOP_SOUND, {}, {}, audioSource});
    }

    void PlayAmbient(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({PLAY_AMBIENT, {}, {}, audioSource});
    }

    void StopAmbient(Hazel::Audio::Source* audioSource)
    {
        instructions.push_back({STOP_AMBIENT, {}, {}, audioSource});
    }

    constexpr Instruction& GetCurrentInstruction()
    {
        return instructions[currentIt];
    }
} // namespace oe::VisualNovel
