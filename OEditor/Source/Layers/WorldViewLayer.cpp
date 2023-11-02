//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "WorldViewLayer.hpp"
#include "Oneiro/Common/JobSystem.hpp"

#include "imgui_stdlib.h"

void OEditor::WorldViewLayer::OnCreate() {}

void OEditor::WorldViewLayer::OnDestroy() {}

void OEditor::WorldViewLayer::OnBegin() {}

void OEditor::WorldViewLayer::OnUpdate(float /*deltaTime*/)
{
	const auto& world = oe::WorldManager::Get()->GetWorld();
	const auto& worldRoot = world->GetRoot();
	ImGui::Begin("World View");
	ImGui::OpenPopupOnItemClick("CreateSmthPopup", ImGuiPopupFlags_MouseButtonRight);

	worldRoot.children([this](flecs::entity flecsEntity) {
		oe::World::Entity entity{flecsEntity, oe::WorldManager::Get()->GetWorld()};
		auto entityName = entity.GetName();

		if (ImGui::Selectable(entityName.c_str(), entity == mSelectedEntity, ImGuiSelectableFlags_::ImGuiSelectableFlags_SpanAllColumns))
			mSelectedEntity = entity;
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered() && !ImGui::IsItemHovered())
			mSelectedEntity = {};
	});

	if (ImGui::BeginPopupContextWindow("CreateSmthPopup"))
	{
		ImGui::BeginDisabled(mSelectedEntity.IsValid());
		if (ImGui::Selectable("Create Entity"))
		{
			oe::JobSystem::Execute([]() {
				int idx{};
				while (oe::WorldManager::Get()->GetWorld()->HasEntity(fmt::format("Entity_{}", idx)))
				{
					idx++;
				}
				oe::WorldManager::Get()->GetWorld()->CreateEntity(fmt::format("Entity_{}", idx));
			});
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(!mSelectedEntity.IsValid());
		if (ImGui::Selectable("Destroy Entity"))
		{
			oe::WorldManager::Get()->GetWorld()->DestroyEntity(mSelectedEntity);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndDisabled();
		ImGui::EndPopup();
	}

	ImGui::End();
}

void OEditor::WorldViewLayer::OnEvent(const oe::Event::Base& baseEvent) {}

void OEditor::WorldViewLayer::OnEnd() {}

oe::World::Entity OEditor::WorldViewLayer::GetSelectedEntity() noexcept
{
	return mSelectedEntity;
}

void OEditor::WorldViewLayer::SetSelectedEntity(oe::World::Entity entity) noexcept
{
	mSelectedEntity = entity;
}
