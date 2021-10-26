#pragma once
#include <random>

namespace Armed
{

class Rand
{
public:
	using RandResultType= uint32_t;

	explicit Rand(RandResultType seed= 0);

	RandResultType Next();

	RandResultType RandValue(RandResultType next_value_after_max);
	// Random value in range [ min_value; next_value_after_max ) with linear distribution.
	RandResultType RandValue(RandResultType min_value, RandResultType next_value_after_max);

	// Returns "true" with chance= 0.5.
	bool RandBool();
	// Returns "true" with chanse= 1 / denominator.
	bool RandBool(RandResultType chance_denominator);
	// Returns "true" with chanse= chance_numerator / denominator.
	bool RandBool(RandResultType chance_numerator, RandResultType chance_denominator);

private:
	// Simple and fast generator.
	// Produces good result for bits 0-31.
	// Parameters, same as in rand() from glibc.
	using Generator= std::linear_congruential_engine< RandResultType, 1103515245u, 12345u, 1u << 31u > ;
	using ExtendedType= uint64_t;

public:
	static constexpr RandResultType c_max_rand_plus_one_= Generator::modulus;

private:
	Generator generator_;
};

} // namespace Armed
