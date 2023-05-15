#pragma once
#include <vector>
#include "../../own-math-3d/math_3d/color.hpp"


namespace util {
    class TGAParser final {
    public:
        static bool Parse(const char* filepath, 
            uint32_t& width, 
            uint32_t& height,
            uint32_t& chanel_count,
            std::vector<math::Color>& colors
        ) noexcept;
    };
}