#pragma once

namespace Armed
{

using fixed16_t = int32_t;

using fixed16vec2_t = fixed16_t[2];

const int32_t g_fixed16_base = 16;

const fixed16_t g_fixed16_one= 1 << 16;

inline int32_t Fixed16FloorToInt(const fixed16_t f)
{
	return f >> g_fixed16_base;
}

inline int32_t Fixed16RoundToInt(const fixed16_t f)
{
	return (f + (1 << (g_fixed16_base - 1))) >> g_fixed16_base;
}

inline int32_t Fixed16CeilToInt(const fixed16_t f)
{
	return (f + (g_fixed16_one - 1)) >> g_fixed16_base;
}

inline fixed16_t IntToFixed16(const int32_t i)
{
	return i << g_fixed16_base;
}

inline fixed16_t Fixed16Mul(const  fixed16_t l, const  fixed16_t r)
{
	return fixed16_t((int64_t(l) * int64_t(r)) >> g_fixed16_base);
}

inline fixed16_t Fixed16Div(const  fixed16_t l, const  fixed16_t r)
{
	return fixed16_t((int64_t(l) << g_fixed16_base) / int64_t(r));
}

inline fixed16_t Fixed16Abs(const fixed16_t x)
{
	return x >= 0 ? x : -x;
}

} // namespace Armed
