#include "Player.hpp"
#include "Assert.hpp"

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

	// Acceleration, friction, max speed - per 1 game tick.
	const fixed16_t acceleration= g_fixed16_one / 128, friction= g_fixed16_one / 256, max_vel= g_fixed16_one / 8;

	// Calculate acceleration vector.
	if(input_state.keyboard[size_t(key_left)])
		accel_vec[0]+= g_fixed16_one;
	if(input_state.keyboard[size_t(key_right)])
		accel_vec[0]-= g_fixed16_one;
	if(input_state.keyboard[size_t(key_down)])
		accel_vec[1]+= g_fixed16_one;
	if(input_state.keyboard[size_t(key_up)])
		accel_vec[1]-= g_fixed16_one;

	{ // Normalize acceleration vector.
		const fixed16_t accel_vec_square_len= Fixed16VecSquareLen(accel_vec);
		if(accel_vec_square_len > 0)
		{
			const fixed16_t len= Fixed16Sqrt(accel_vec_square_len);
			accel_vec[0]= Fixed16Div(accel_vec[0], len);
			accel_vec[1]= Fixed16Div(accel_vec[1], len);
		}
	}

	{ // Accelerate, but limit result projection of current velocity to wish direction.
		const fixed16_t vel_proj= Fixed16VecDot(vel_, accel_vec); // Length of projection of current speed on wish direction.
		if(vel_proj < max_vel)
		{
			const fixed16_t add_speed= std::min(max_vel - vel_proj, acceleration);
			vel_[0]+= Fixed16Mul(add_speed, accel_vec[0]);
			vel_[1]+= Fixed16Mul(add_speed, accel_vec[1]);
		}
	}

	// Apply frction.
	const fixed16_t vel_modulo= Fixed16Sqrt(Fixed16VecSquareLen(vel_));
	if(vel_modulo > 0)
	{
		const fixed16_t cur_friction= std::min(vel_modulo, friction);
		const fixed16vec2_t prev_vel{vel_[0], vel_[1]};
		for(uint32_t i= 0; i < 2; ++i)
			vel_[i]-= Fixed16Div(Fixed16Mul(vel_[i], cur_friction), vel_modulo);

		// Fix calculation errors - make velosity zero in case of new velosity points in another direction.
		if( Fixed16VecDot(vel_, prev_vel) < 0)
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

	const fixed16_t vel_dir_dot= Fixed16VecDot(push_vec, vel_);
	if(vel_dir_dot < 0)
	{
		// Clamp velosity in case of opposing push.
		const fixed16_t push_vec_square_len= Fixed16VecSquareLen(push_vec);
		if(push_vec_square_len > 0)
		{
			const fixed16_t dot_corrected= Fixed16Div(vel_dir_dot, push_vec_square_len);
			vel_[0]-= Fixed16Mul(push_vec[0], dot_corrected);
			vel_[1]-= Fixed16Mul(push_vec[1], dot_corrected);
		}
	}
}

} // namespace Armed
