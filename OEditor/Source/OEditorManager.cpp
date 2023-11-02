//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "OEditorManager.hpp"

OEditor::Manager* OEditor::Manager::Get() noexcept
{
	static Manager manager{};
	return &manager;
}

void OEditor::Manager::SetActiveProject(const oe::Ref<oe::Project>& project)
{
	mActiveProject = project;
}

const oe::Ref<oe::Project>& OEditor::Manager::GetActiveProject()
{
	return mActiveProject;
}
