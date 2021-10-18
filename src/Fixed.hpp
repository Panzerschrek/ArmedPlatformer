#pragma once

namespace Armed
{

using fixed16_t = int32_t;

using fixed16vec2_t = fixed16_t[2];

const fixed16_t g_fixed16_one= 1 << 16;

inline int32_t Fixed16FloorToInt(const fixed16_t f)
{
	return f >> 16;
}

} // namespace Armed
