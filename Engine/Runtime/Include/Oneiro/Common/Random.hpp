//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <random>

namespace oe
{
	class Random
	{
	public:
		static uint64_t DiceID() noexcept;

		static int32_t DiceInt(int32_t min, int32_t max) noexcept;

		static float DiceFloat(float min, float max) noexcept;

		static double DiceDouble(double min, double max) noexcept;

	private:
		inline static std::random_device mRandomDevice{};
		inline static std::default_random_engine mRandomEngine{mRandomDevice()};
	};
} // namespace oe
