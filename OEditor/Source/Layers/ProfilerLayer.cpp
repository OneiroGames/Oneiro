//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "ProfilerLayer.hpp"

void OEditor::ProfilerLayer::OnCreate() {}

void OEditor::ProfilerLayer::OnDestroy() {}

void OEditor::ProfilerLayer::OnBegin() {}

void OEditor::ProfilerLayer::OnUpdate(float deltaTime)
{
	ImGui::Begin("Profiler");

	if (mRefreshTime == 0.0)
		mRefreshTime = ImGui::GetTime();

	while (mRefreshTime < ImGui::GetTime())
	{
		mValues[mValuesOffset] = deltaTime * 1000.0f;
		mValuesOffset = (mValuesOffset + 1) % 90;
		mRefreshTime += 1.0f / 60.0f;
	}

	{
		float average{};
		for (const auto value : mValues)
			average += value;
		average /= 90.0f;
		ImGui::PlotLines("Frame Time", mValues.data(), 90, mValuesOffset, fmt::format("Average: {:.4f}", average).c_str(), -1.0f, 30.0f,
						 ImVec2(0, 80.0f));
	}
	ImGui::End();
}

void OEditor::ProfilerLayer::OnEvent(const oe::Event::Base& baseEvent)
{
	Layer::OnEvent(baseEvent);
}

void OEditor::ProfilerLayer::OnEnd()
{
	Layer::OnEnd();
}
