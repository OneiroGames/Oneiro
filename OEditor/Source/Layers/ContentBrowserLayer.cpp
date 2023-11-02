//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "ContentBrowserLayer.hpp"
#include "../OEditorManager.hpp"

#include "Oneiro/Common/FileSystem/Path.hpp"
#include "Oneiro/Core/Assets/AssetsManager.hpp"
#include "Oneiro/World/Components/AudioSourceComponent.hpp"
#include "Oneiro/World/WorldManager.hpp"
#include "imgui.h"

void OEditor::ContentBrowserLayer::OnCreate()
{
	LoadAssets();
	mBasePath = oe::FileSystem::CurrentPath();
	mCurrentPath = mBasePath;
}

void OEditor::ContentBrowserLayer::OnDestroy()
{
	UnLoadAssets();
}

void OEditor::ContentBrowserLayer::OnBegin() {}

void OEditor::ContentBrowserLayer::OnUpdate(float /*deltaTime*/)
{
	ImGui::Begin("Content Browser");

	ImGui::BeginDisabled(mCurrentPath == mBasePath);
	if (ImGui::Button("<-"))
	{
		mCurrentPath = mCurrentPath.parent_path();
	}
	ImGui::EndDisabled();

	static constexpr float padding = 16.0f;
	static constexpr float thumbnailSize = 96.0f;
	const auto cellSize = thumbnailSize + padding;

	const auto panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = static_cast<int>(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, nullptr, false);

	const auto& pathIterator = oe::FileSystem::Path::Iterator(mCurrentPath);
	for (const auto& pathEntry : pathIterator)
	{
		const oe::FileSystem::Path path{pathEntry.path()};
		const auto& filenameString = path.filename().string();

		ImGui::PushID(filenameString.c_str());
		const auto& icon = pathEntry.is_directory() ? mPathTexture : mFileTexture;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton(reinterpret_cast<void*>(static_cast<size_t>(icon->Get()->GetId())), {thumbnailSize, thumbnailSize}, {0, 1}, {1, 0});
		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (pathEntry.is_directory())
				mCurrentPath /= path.filename();

			if (path.extension() == ".oeproject")
			{
				auto backup = oe::Project::GetActive();
				if (const auto& active = Manager::Get()->GetActiveProject())
				{
					active->Save((oe::FileSystem::Path("/") / active->GetPath()).string());
					if (oe::WorldManager::Get()->GetWorld()->IsLoaded())
						oe::WorldManager::Get()->UnLoadWorld();
				}
				auto loaded = oe::Project::Load((oe::FileSystem::Path("/") / path.GetLocal()));
				Manager::Get()->SetActiveProject(loaded);
				const auto& assetsPath = loaded->GetAssetsPath().string();
				if (oe::FileSystem::IsFile("/" / loaded->GetWorldsPath() / loaded->GetStartWorldPath()))
				{
					oe::Project::GetActive()->SetAssetsPath((oe::FileSystem::Path("/") / assetsPath));
					oe::WorldManager::Get()->LoadWorld("/" / loaded->GetWorldsPath() / loaded->GetStartWorldPath());

					auto filter = oe::WorldManager::Get()->GetWorld()->GetHandle()->filter<oe::World::Components::AudioSource>();
					filter.each([this](flecs::entity /*flecsEntity*/, oe::World::Components::AudioSource& audioSource) {
						if (audioSource.GetAudioSource()->IsPlaying())
						{
							audioSource.GetAudioSource()->Stop();
						}
					});

					oe::Project::GetActive()->SetAssetsPath(assetsPath);
				}
				oe::Project::SetActive(backup);
			}

			if (path.extension() == ".oeworld" && Manager::Get()->GetActiveProject())
			{
				auto backupProject = oe::Project::GetActive();
				oe::Project::SetActive(Manager::Get()->GetActiveProject());
				const auto& assetsPath = oe::Project::GetActive()->GetAssetsPath().string();
				oe::Project::GetActive()->SetAssetsPath((oe::FileSystem::Path("/") / assetsPath));
				oe::WorldManager::Get()->LoadWorld((oe::FileSystem::Path("/") / path.GetLocal()));

				auto filter = oe::WorldManager::Get()->GetWorld()->GetHandle()->filter<oe::World::Components::AudioSource>();
				filter.each([this](flecs::entity /*flecsEntity*/, oe::World::Components::AudioSource& audioSource) {
					if (audioSource.GetAudioSource()->IsPlaying())
					{
						audioSource.GetAudioSource()->Stop();
					}
				});

				oe::Project::GetActive()->SetAssetsPath(assetsPath);
				oe::Project::SetActive(backupProject);
			}
		}

		const auto dragAndDropSrc = [=](const std::string& type) {
			if (ImGui::BeginDragDropSource())
			{
				const auto& itemPathStr = path.GetLocal().string();
				const auto& itemPath = itemPathStr.c_str();
				ImGui::SetDragDropPayload(type.c_str(), itemPath, (std::strlen(itemPath) + 1) * sizeof(char));
				ImGui::EndDragDropSource();
			}
		};
		const auto& extension = path.extension();
		if (extension == ".png" || extension == ".jpg")
		{
			dragAndDropSrc("TextureItem");
		}

		if (extension == ".mp3" || extension == ".ogg")
		{
			dragAndDropSrc("AudioItem");
		}

		ImGui::TextWrapped("%s", filenameString.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}

	ImGui::Columns(1);

	ImGui::End();
}

void OEditor::ContentBrowserLayer::OnEvent(const oe::Event::Base& baseEvent) {}

void OEditor::ContentBrowserLayer::OnEnd() {}

void OEditor::ContentBrowserLayer::LoadAssets() noexcept
{
	mFileTexture = oe::AssetsManager::GetOrCreateAsset<oe::Renderer2D::Texture>("ContentBrowserFileIcon", oe::FileSystem::Path("Icons/FileIcon.png"));
	mPathTexture = oe::AssetsManager::GetOrCreateAsset<oe::Renderer2D::Texture>("ContentBrowserPathIcon", oe::FileSystem::Path("Icons/PathIcon.png"));

	oe::AssetsManager::LoadAsset(mFileTexture);
	oe::AssetsManager::LoadAsset(mPathTexture);
}

void OEditor::ContentBrowserLayer::UnLoadAssets() noexcept
{
	mFileTexture.reset();
	mPathTexture.reset();
	oe::AssetsManager::CollectGarbage();
}
