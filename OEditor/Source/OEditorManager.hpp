//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include "Project/Project.hpp"

namespace OEditor
{
	class Manager
	{
	public:
		static Manager* Get() noexcept;

		void SetActiveProject(const oe::Ref<oe::Project>& project);

		const oe::Ref<oe::Project>& GetActiveProject();

	private:
		oe::Ref<oe::Project> mActiveProject{};
	};
} // namespace OEditor
