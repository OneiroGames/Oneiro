//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "EditorSettingsLayer.hpp"
#include "Audio/AudioSystem.hpp"

void OEditor::EditorSettingsLayer::OnCreate()
{
	oe::Audio::System::SetGlobalVolume(0.1f);
}

void OEditor::EditorSettingsLayer::OnDestroy() {}

void OEditor::EditorSettingsLayer::OnBegin() {}

void OEditor::EditorSettingsLayer::OnUpdate(float /*deltaTime*/)
{
	ImGui::Begin("Editor Settings");

	float audioGlobalVolume = oe::Audio::System::GetGlobalVolume();
	if (ImGui::DragFloat("Global Audio Volume", &audioGlobalVolume, 0.001f, 0.0f, 1.0f))
	{
		oe::Audio::System::SetGlobalVolume(audioGlobalVolume);
	}
	ImGui::End();
}

void OEditor::EditorSettingsLayer::OnEvent(const oe::Event::Base& baseEvent) {}

void OEditor::EditorSettingsLayer::OnEnd() {}
