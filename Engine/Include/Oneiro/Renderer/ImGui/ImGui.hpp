//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/EngineApi.hpp"

// clang-format off
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"
// clang-format on

namespace oe
{
	ImGuiContext* g_ImGuiContext{};
	inline bool InitializeImGui()
	{
		g_ImGuiContext = ImGui::CreateContext();
		ImGuiIO& guiIo = ImGui::GetIO();
		guiIo.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		// guiIo.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		guiIo.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		guiIo.DisplaySize = {1920, 1080};

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		style.ScrollbarSize = 8;
		style.ScrollbarRounding = 12;
		style.WindowTitleAlign.x = 0.5f;
		style.WindowRounding = 6;
		if (guiIo.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		EngineApi::GetWindowManager()->InitializeImGui(g_ImGuiContext);
		EngineApi::GetRendererBackend()->InitializeImGui(g_ImGuiContext);
		return true;
	}

	inline bool BeginImGuiFrame()
	{
		EngineApi::GetWindowManager()->BeginImGuiFrame();
		EngineApi::GetRendererBackend()->BeginImGuiFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		return true;
	}

	inline bool EndImGuiFrame()
	{
		ImGui::Render();
		EngineApi::GetRendererBackend()->EndImGuiFrame();
		EngineApi::GetWindowManager()->EndImGuiFrame();
		return true;
	}

	inline bool ShutdownImGui()
	{
		EngineApi::GetRendererBackend()->ShutdownImGui();
		EngineApi::GetWindowManager()->ShutdownImGui();
		ImGui::DestroyContext();
		return true;
	}
} // namespace oe