#include "Player.hpp"
#include "Assert.hpp"
#include <cmath> // for sqrt

namespace Armed
{

Player::Player(const fixed16_t pos_x, const fixed16_t pos_y)
	: pos_{pos_x, pos_y}, vel_{0, 0}
{
}

void Player::Tick(const InputState& input_state)
{
	using KeyCode= SystemEventTypes::KeyCode;
	const auto key_left = KeyCode::D;
	const auto key_right = KeyCode::A;
	const auto key_up = KeyCode::W;
	const auto key_down = KeyCode::S;

	fixed16vec2_t accel_vec{0, 0};

	const fixed16_t acceleration= g_fixed16_one / 128, friction= g_fixed16_one / 256, max_vel= g_fixed16_one / 4;

	if(input_state.keyboard[size_t(key_left)])
		accel_vec[0]+= acceleration;
	if(input_state.keyboard[size_t(key_right)])
		accel_vec[0]-= acceleration;
	if(input_state.keyboard[size_t(key_down)])
		accel_vec[1]+= acceleration;
	if(input_state.keyboard[size_t(key_up)])
		accel_vec[1]-= acceleration;

	for(uint32_t i= 0; i < 2; ++i)
		vel_[i]+= accel_vec[i];

	const fixed16_t square_vel= Fixed16Mul(vel_[0], vel_[0]) + Fixed16Mul(vel_[1], vel_[1]);
	const fixed16_t vel_modulo= fixed16_t(std::sqrt(float(square_vel) / float(g_fixed16_one)) * float(g_fixed16_one));

	if(vel_modulo > max_vel)
	{
		// Limit speed.
		const fixed16_t scaler= Fixed16Div(max_vel, vel_modulo);
		for(uint32_t i= 0; i < 2; ++i)
			vel_[i]= Fixed16Mul(vel_[i], scaler);
	}
	else if(vel_modulo > 0)
	{
		// Apply acceleration.
		const fixed16_t cur_friction= std::min(vel_modulo, friction);
		for(uint32_t i= 0; i < 2; ++i)
			vel_[i]-= Fixed16Div(Fixed16Mul(vel_[i], cur_friction), vel_modulo);
	}
	else
		vel_[0]= vel_[1]= 0; // Zero small speed value.

	for(uint32_t i= 0; i < 2; ++i)
		pos_[i]+= vel_[i];
}

} // namespace Armed
