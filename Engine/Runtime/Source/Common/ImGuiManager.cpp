//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

module;

#include "Oneiro/Common/StdAfx.hpp"

// clang-format off
#include "imgui.h"
#include "imgui_internal.h"
//#include "ImGuizmo.h"
// clang-format on

module Oneiro.Common.ImGuiManager;

import Oneiro.Common.EngineApi;

bool oe::ImGuiManager::Initialize()
{
	m_Context = ImGui::CreateContext();
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
	EngineApi::GetWindowManager()->InitializeImGui();
	EngineApi::GetRHI()->InitializeImGui();
	return true;
}

bool oe::ImGuiManager::BeginFrame()
{
	EngineApi::GetWindowManager()->BeginImGuiFrame();
	EngineApi::GetRHI()->BeginImGuiFrame();
	ImGui::NewFrame();
	//ImGuizmo::BeginFrame();

	return true;
}

bool oe::ImGuiManager::EndFrame()
{
	ImGui::Render();
	EngineApi::GetRHI()->EndImGuiFrame();
	EngineApi::GetWindowManager()->EndImGuiFrame();
	return true;
}

bool oe::ImGuiManager::Shutdown()
{
	EngineApi::GetRHI()->ShutdownImGui();
	EngineApi::GetWindowManager()->ShutdownImGui();
	ImGui::DestroyContext();
	return true;
}