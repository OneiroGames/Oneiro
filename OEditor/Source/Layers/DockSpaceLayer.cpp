//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "DockSpaceLayer.hpp"

void OEditor::DockSpaceLayer::OnCreate() {}

void OEditor::DockSpaceLayer::OnDestroy() {}

void OEditor::DockSpaceLayer::OnBegin() {}

void OEditor::DockSpaceLayer::OnUpdate(float /*deltaTime*/)
{
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
}

void OEditor::DockSpaceLayer::OnEvent(const oe::Event::Base& baseEvent) {}

void OEditor::DockSpaceLayer::OnEnd() {}
