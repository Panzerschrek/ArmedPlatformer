#pragma once
#include <cstdint>

namespace Armed
{

using color_t= uint32_t;

inline constexpr color_t ColorRGB(const uint8_t r, const uint8_t g, const uint8_t b)
{
	return color_t((b << 0u) | (g << 8u) | (r << 16u));
}

} // namespace Armed
