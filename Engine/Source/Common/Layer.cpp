//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/Layer.hpp"

oe::Layer::Layer(std::string name) : mName(std::move(name)) {}

void oe::Layer::OnCreate() {}

void oe::Layer::OnDestroy() {}

void oe::Layer::OnBegin() {}

void oe::Layer::OnUpdate(float deltaTime) {}

void oe::Layer::OnEnd() {}
