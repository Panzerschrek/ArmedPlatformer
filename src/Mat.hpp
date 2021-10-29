#pragma once
#include "Fixed.hpp"

namespace Armed
{

// new_x = x * scale[0] + shift[0]
// new_y = y * scale[1] + shift[1]
struct TransformMatrix
{
	fixed16_t shift[2];
	fixed16_t scale[2];
};

inline TransformMatrix MatrixMul(const TransformMatrix& l, const TransformMatrix& r)
{
	TransformMatrix res{};
	res.scale[0] = Fixed16Mul(l.scale[0], r.scale[0]);
	res.scale[1] = Fixed16Mul(l.scale[1], r.scale[1]);
	res.shift[0]= Fixed16Mul(l.shift[0], r.scale[0]) + r.shift[0];
	res.shift[1]= Fixed16Mul(l.shift[1], r.scale[1]) + r.shift[1];
	return res;
}

inline fixed16vec2_t VecMatMul(const fixed16vec2_t& v, const TransformMatrix& mat)
{
	return { Fixed16Mul(mat.scale[0], v[0]) + mat.shift[0], Fixed16Mul(mat.scale[1], v[1]) + mat.shift[1]};
}

} // namespace Armed