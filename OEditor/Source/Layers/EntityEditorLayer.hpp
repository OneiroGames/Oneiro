//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Layer.hpp"
#include "WorldViewLayer.hpp"

#include "imgui_internal.h"

namespace OEditor
{
	class EntityEditorLayer final : public oe::Layer
	{
	public:
		using oe::Layer::Layer;

		void OnCreate() override;
		void OnDestroy() override;

		void OnBegin() override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(const oe::Event::Base& baseEvent) override;
		void OnEnd() override;

	private:
		void AcceptContentBrowserPayload(const std::string& type, const std::function<void(const std::string&)>& func) const;

		template <typename T>
		void DisplayAddComponentEntry(oe::World::Entity entity, const std::string& entryName);

		void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 80.0f);
		template <typename T>
		void DrawComponent(const std::string& name, oe::World::Entity entity, const std::function<void(T*)>& uiFunction, bool isRemovable = true,
						   bool isCanChangeStatus = true);
	};

	template <typename T>
	void EntityEditorLayer::DisplayAddComponentEntry(oe::World::Entity entity, const std::string& entryName)
	{
		if (!entity.HasComponent<T>())
		{
			if (ImGui::MenuItem(entryName.c_str()))
			{
				entity.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	template <typename T>
	void EntityEditorLayer::DrawComponent(const std::string& name, oe::World::Entity entity, const std::function<void(T*)>& uiFunction,
										  bool isRemovable, bool isCanChangeStatus)
	{
		const auto treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
								   ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			const auto& component = entity.GetComponent<T>();
			const auto& contentRegionAvailable = ImGui::GetContentRegionAvail();
			const auto& lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
			ImGui::Separator();
			const auto& open = ImGui::TreeNodeEx(reinterpret_cast<void*>(typeid(T).hash_code()), treeNodeFlags, "%s", name.c_str());
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{lineHeight, lineHeight}))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent{};
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				ImGui::BeginDisabled(!isRemovable);
				if (ImGui::MenuItem("Remove component"))
					removeComponent = true;
				ImGui::EndDisabled();

				ImGui::BeginDisabled(!isCanChangeStatus);
				if (component->IsActive())
				{
					if (ImGui::MenuItem("Disable component"))
						component->SetActive(false);
				}
				else
				{
					if (ImGui::MenuItem("Enable component"))
						component->SetActive(true);
				}
				ImGui::EndDisabled();

				ImGui::EndPopup();
			}

			if (open)
			{
				ImGui::BeginDisabled(!component->IsActive());
				uiFunction(component);
				ImGui::EndDisabled();
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}
} // namespace OEditor
