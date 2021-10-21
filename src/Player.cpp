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
		const fixed16vec2_t prev_vel{vel_[0], vel_[1]};
		for(uint32_t i= 0; i < 2; ++i)
			vel_[i]-= Fixed16Div(Fixed16Mul(vel_[i], cur_friction), vel_modulo);

		// Fix calculation errors - make velosity zero in case of new velosity points in another direction.
		if(Fixed16Mul(vel_[0], prev_vel[0]) + Fixed16Mul(vel_[1], prev_vel[1]) < 0)
			vel_[0]= vel_[1]= 0;
	}
	else
		vel_[0]= vel_[1]= 0; // Zero small speed value.

	for(uint32_t i= 0; i < 2; ++i)
		pos_[i]+= vel_[i];
}

void Player::Push(const fixed16vec2_t& push_vec)
{
	pos_[0]+= push_vec[0];
	pos_[1]+= push_vec[1];

	const fixed16_t vel_dir_dot= Fixed16Mul(push_vec[0], vel_[0]) + Fixed16Mul(push_vec[1], vel_[1]);
	if(vel_dir_dot < 0)
	{
		// Clamp velosity in case of opposing push.
		const fixed16_t push_vec_square_len= Fixed16Mul(push_vec[0], push_vec[0]) + Fixed16Mul(push_vec[1], push_vec[1]);
		if(push_vec_square_len > 0)
		{
			const fixed16_t dot_corrected= Fixed16Div(vel_dir_dot, push_vec_square_len);
			vel_[0]-= Fixed16Mul(push_vec[0], dot_corrected);
			vel_[1]-= Fixed16Mul(push_vec[1], dot_corrected);
		}
	}
}

} // namespace Armed
