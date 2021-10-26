#include "Rand.hpp"
#include "Assert.hpp"

namespace Armed
{

Rand::Rand(const RandResultType seed)
	: generator_(seed)
{}

Rand::RandResultType Rand::Next()
{
	return generator_();
}

Rand::RandResultType Rand::RandValue(const RandResultType next_value_after_max)
{
	return RandResultType(ExtendedType(Next()) * ExtendedType(next_value_after_max) / ExtendedType(c_max_rand_plus_one_));
}

Rand::RandResultType Rand::RandValue(const RandResultType min_value, const RandResultType next_value_after_max)
{
	ARMED_ASSERT(min_value <= next_value_after_max);
	return RandValue(next_value_after_max - min_value) + min_value;
}

bool Rand::RandBool()
{
	return (Next() & 1u) != 0u;
}

bool Rand::RandBool(const RandResultType chance_denominator)
{
	return RandBool(1u, chance_denominator);
}

bool Rand::RandBool(const RandResultType chance_numerator, const RandResultType chance_denominator)
{
	ARMED_ASSERT( chance_numerator <= chance_denominator );

	return
		ExtendedType(Next()) * ExtendedType(chance_denominator) <
		ExtendedType(c_max_rand_plus_one_) * ExtendedType(chance_numerator);
}

} // namespace PanzerChasm
