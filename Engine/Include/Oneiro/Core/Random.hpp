//
// Copyright (c) Oneiro Games. All rights reserved.
// Licensed under the GNU General Public License, Version 3.0.
//

#pragma once

#include <random>

namespace oe::Core
{
    class Random
    {
      public:
        static int DiceInt(int min, int max);
        static float DiceFloat(float min, float max);
        static float DiceFloat();
        static double DiceDouble(double min, double max);

        static std::string DiceUuid();

      private:
        inline static std::mt19937 mGenerator{};
        inline static std::uniform_int_distribution<> mDis{0, 15};
        inline static std::uniform_int_distribution<> mDis2{8, 11};
    };
} // namespace oe::Core
