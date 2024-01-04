//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#include "Oneiro/Common/Random.hpp"

namespace oe
{
	uint64_t Random::DiceID() noexcept
	{
		std::uniform_int_distribution<uint64_t> uniformIntDistribution{0, std::numeric_limits<uint64_t>::max()};
		return uniformIntDistribution(mRandomEngine);
	}

	int32_t Random::DiceInt(int32_t min, int32_t max) noexcept
	{
		std::uniform_int_distribution<int32_t> uniformIntDistribution{min, max};
		return uniformIntDistribution(mRandomEngine);
	}

	float Random::DiceFloat(float min, float max) noexcept
	{
		std::uniform_real_distribution<float> uniformRealDistribution{min, max};
		return uniformRealDistribution(mRandomEngine);
	}

	double Random::DiceDouble(double min, double max) noexcept
	{
		std::uniform_real_distribution<double> uniformRealDistribution{min, max};
		return uniformRealDistribution(mRandomEngine);
	}
} // namespace oe