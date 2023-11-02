//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/Layer.hpp"
#include "Oneiro/Common/Signals/Dispatcher.hpp"
#include "Oneiro/Common/Signals/Events.hpp"
#include "Oneiro/Core/WM/WindowManager.hpp"
#include "Oneiro/Renderer/Camera.hpp"
#include "Oneiro/Renderer/OpenGL/GLFrameBuffer.hpp"
#include "Oneiro/World/CameraController/CameraController2D.hpp"
#include "Oneiro/World/Entity.hpp"

#include "ImGuizmo.h"
#include "imgui.h"

namespace OEditor
{
	class ViewportLayer final : public oe::Layer
	{
	public:
		using oe::Layer::Layer;

		void OnCreate() override;
		void OnDestroy() override;

		void OnBegin() override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(const oe::Event::Base& baseEvent) override;
		void OnEnd() override;

		[[nodiscard]] const oe::Ref<oe::Renderer::Camera>& GetCamera() noexcept;
		[[nodiscard]] const oe::Ref<oe::World::CameraController2D>& GetCameraController() noexcept;

		oe::World::Entity GetHoveredEntity() noexcept;

	private:
		bool HandleWindowResize();

		oe::Renderer::GL::FrameBuffer mFrameBuffer{};
		oe::Ref<oe::Renderer::Camera> mCamera{};
		oe::Ref<oe::World::CameraController2D> mCameraController{};
		std::array<glm::vec2, 2> mViewportBounds;
		oe::World::Entity mHoveredEntity{};
		ImGuizmo::OPERATION mGizmoType{ImGuizmo::TRANSLATE};

		bool mIsViewportHovered{};
		bool mIsViewportFocused{};

		bool mIsImguizmoHovered{};
		bool mIsImguizmoUsing{};
	};
} // namespace OEditor
