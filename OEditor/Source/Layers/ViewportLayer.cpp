//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "ViewportLayer.hpp"
#include "Math/Math.hpp"
#include "Oneiro/Common/LayerManager.hpp"
#include "Oneiro/Renderer/OrthographicCamera.hpp"
#include "WorldViewLayer.hpp"

#include "glm/gtc/type_ptr.hpp"

#include "imgui_internal.h"

void OEditor::ViewportLayer::OnCreate()
{
	mCamera = oe::CreateRef<oe::Renderer::OrthographicCamera>();
	mCameraController = oe::CreateRef<oe::World::CameraController2D>();

	const auto [windowWidth, windowHeight] = oe::WindowManager::GetMainWindow()->GetSize();
	mFrameBuffer.Create({{{{oe::Renderer::GL::TextureAttachmentFormat::RGBA8},
						   {oe::Renderer::GL::TextureAttachmentFormat::RED_INTEGER},
						   {oe::Renderer::GL::TextureAttachmentFormat::DEPTH24STENCIL8}}},
						 static_cast<uint32_t>(windowWidth),
						 static_cast<uint32_t>(windowHeight)});

	glLineWidth(4.0f);

	oe::Event::Dispatcher::Subscribe<oe::Event::Renderer2D::DrawFrame>([this](const auto& /*baseEvent*/) {
		mFrameBuffer.Bind();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});

	oe::Event::Dispatcher::Subscribe<oe::Event::Renderer2D::EndFrame>([this](const auto& /*baseEvent*/) {
		auto [mx, my] = ImGui::GetMousePos();
		mx -= mViewportBounds[0].x;
		my -= mViewportBounds[0].y;
		const auto viewportSize = mViewportBounds[1] - mViewportBounds[0];
		my = viewportSize.y - my;
		if (mx >= 0.0f && my >= 0.0f && mx < viewportSize.x && my < viewportSize.y && mIsViewportHovered && !mIsImguizmoHovered && !mIsImguizmoUsing)
		{
			const auto hoveredEntityId = mFrameBuffer.ReadPixel(1, {mx, my});
			auto handle = oe::WorldManager::Get()->GetWorld()->GetHandle()->get_alive(hoveredEntityId);
			mHoveredEntity = handle.is_valid() ? oe::World::Entity{handle, oe::WorldManager::Get()->GetWorld()} : oe::World::Entity{};

			if (ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
				WorldViewLayer::SetSelectedEntity(mHoveredEntity);
		}

		mFrameBuffer.UnBind();
	});
}

void OEditor::ViewportLayer::OnDestroy() {}

void OEditor::ViewportLayer::OnBegin() {}

void OEditor::ViewportLayer::OnUpdate(float /*deltaTime*/)
{
	oe::WorldManager::Get()->GetWorld()->OnUpdateRuntime();
	oe::WorldManager::Get()->GetWorld()->OnRender(*mCamera, mCameraController->GetViewMatrix());

	if (!ImGui::GetIO().WantTextInput && !oe::Input::IsKeyPressed(oe::Input::Key::LCTRL))
	{
		if (oe::Input::IsKeyPressed(oe::Input::Key::W))
			mCameraController->UpdateForward();
		if (oe::Input::IsKeyPressed(oe::Input::Key::S))
			mCameraController->UpdateBackward();
		if (oe::Input::IsKeyPressed(oe::Input::Key::A))
			mCameraController->UpdateLeft();
		if (oe::Input::IsKeyPressed(oe::Input::Key::D))
			mCameraController->UpdateRight();

		if (oe::Input::IsKeyPressed(oe::Input::Key::R))
			mGizmoType = ImGuizmo::ROTATE;
		if (oe::Input::IsKeyPressed(oe::Input::Key::E))
			mGizmoType = ImGuizmo::SCALE;
		if (oe::Input::IsKeyPressed(oe::Input::Key::T))
			mGizmoType = ImGuizmo::TRANSLATE;
	}

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Viewport");
	mIsViewportHovered = ImGui::IsWindowHovered();
	mIsViewportFocused = ImGui::IsWindowFocused();
	if (!HandleWindowResize())
	{
		mIsViewportHovered = false;
		mIsViewportFocused = false;
		ImGui::End();
		return;
	}

	const auto& viewportMinRegion = ImGui::GetWindowContentRegionMin();
	const auto& viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	const auto& viewportOffset = ImGui::GetWindowPos();
	const auto& cameraProjection = mCamera->GetProjection();
	const auto& cameraView = mCameraController->GetViewMatrix();
	auto selectedEntity = WorldViewLayer::GetSelectedEntity();

	mViewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
	mViewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

	ImGui::Image(reinterpret_cast<void*>(mFrameBuffer.GetColorAttachmentRendererID()),
				 {static_cast<float>(mFrameBuffer.GetSize().x), static_cast<float>(mFrameBuffer.GetSize().y)}, ImVec2(0.0f, 1.0f),
				 ImVec2(1.0f, 0.0f));

	ImGuizmo::SetOrthographic(true);
	ImGuizmo::SetDrawlist();
	ImGuizmo::SetRect(mViewportBounds[0].x, mViewportBounds[0].y, mViewportBounds[1].x - mViewportBounds[0].x,
					  mViewportBounds[1].y - mViewportBounds[0].y);

	if (selectedEntity.IsValid())
	{
		const auto& transformComponent = selectedEntity.GetComponent<oe::World::Components::Transform>();
		auto transform = transformComponent->GetTransform();

		const auto snap = oe::Input::IsKeyPressed(oe::Input::Key::LCTRL);
		glm::vec3 snapValues{0.25f};
		if (mGizmoType == ImGuizmo::ROTATE)
			snapValues = {45.0f, 45.0f, 45.0f};

		ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), mGizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
							 nullptr, snap ? glm::value_ptr(snapValues) : nullptr);

		if (ImGuizmo::IsUsing())
		{
			glm::vec3 translation{};
			glm::vec3 rotation{};
			glm::vec3 scale{};

			oe::Math::DecomposeTransform(transform, translation, rotation, scale);

			transformComponent->position = translation;
			transformComponent->rotation = rotation;
			transformComponent->scale = scale;
		}
	}
	mIsImguizmoHovered = ImGuizmo::IsOver();
	mIsImguizmoUsing = ImGuizmo::IsUsing();

	ImGui::End();
	ImGui::PopStyleVar();
}

void OEditor::ViewportLayer::OnEvent(const oe::Event::Base& baseEvent)
{
	if (IsSame<oe::Event::Mouse::Wheel>(baseEvent) && mIsViewportHovered)
	{
		const auto& mouseWheelEvent = Cast<oe::Event::Mouse::Wheel>(baseEvent);
		const auto& orthographicCamera = dynamic_cast<oe::Renderer::OrthographicCamera*>(mCamera.get());
		orthographicCamera->zoom -= (mouseWheelEvent.y / 10);
	}
}

void OEditor::ViewportLayer::OnEnd() {}

bool OEditor::ViewportLayer::HandleWindowResize()
{
	const auto& view = ImGui::GetContentRegionAvail();

	if (static_cast<int32_t>(view.x) != mFrameBuffer.GetSize().x || static_cast<int32_t>(view.y) != mFrameBuffer.GetSize().y)
	{
		if (view.x <= 0 || view.y <= 0)
			return false;
		mFrameBuffer.Resize(static_cast<int32_t>(view.x), static_cast<int32_t>(view.y));

		return true;
	}

	return true;
}

const oe::Ref<oe::Renderer::Camera>& OEditor::ViewportLayer::GetCamera() noexcept
{
	return mCamera;
}

const oe::Ref<oe::World::CameraController2D>& OEditor::ViewportLayer::GetCameraController() noexcept
{
	return mCameraController;
}

oe::World::Entity OEditor::ViewportLayer::GetHoveredEntity() noexcept
{
	return mHoveredEntity;
}
