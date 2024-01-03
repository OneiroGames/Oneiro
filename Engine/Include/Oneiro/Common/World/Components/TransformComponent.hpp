//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Oneiro/Common/World/Components/IBaseComponent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

namespace oe
{
	class TransformComponent : public IBaseComponent
	{
	public:
		glm::vec3 position{};
		glm::vec3 scale{1.0f};
		glm::vec3 rotation{};

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale) * glm::toMat4(glm::quat(rotation));
		}

		void Serialize() override {}

		void Deserialize() override {}
	};
} // namespace oe