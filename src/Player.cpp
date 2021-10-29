#include "Player.hpp"
#include "Assert.hpp"

namespace Armed
{

const int32_t c_max_health= 100;

Player::Player(const fixed16_t pos_x, const fixed16_t pos_y)
	: pos_{pos_x, pos_y}, vel_{0, 0}, health_(c_max_health)
{
}

Player::ShootRequestKind Player::Tick(const InputState& input_state)
{
	Move(input_state);
	const auto res= Shoot(input_state);
	++current_tick_;
	return res;
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

void Player::Hit(const int32_t damage)
{
	health_-= damage;
}

void Player::Move(const InputState& input_state)
{
	using KeyCode= SystemEventTypes::KeyCode;
	const auto key_left = KeyCode::D;
	const auto key_right = KeyCode::A;
	const auto key_up = KeyCode::W;
	const auto key_down = KeyCode::S;
	const auto key_jump= KeyCode::Space;

	fixed16vec2_t accel_vec{0, 0};

	// Acceleration, friction, max speed - per 1 game tick.
	const fixed16_t c_gravity= g_fixed16_one / 512;
	const fixed16_t c_air_acceleration= g_fixed16_one / 2048;
	const fixed16_t c_ground_acceleration= g_fixed16_one / 128;
	const fixed16_t c_air_friction= g_fixed16_one / 2048;
	const fixed16_t c_ground_friction= g_fixed16_one / 256;
	const fixed16_t c_terminal_vel= g_fixed16_one;
	const fixed16_t c_max_vel= g_fixed16_one / 8;
	const fixed16_t c_jump_vel= g_fixed16_one / 10;

	if(on_ground_)
	{
		if(input_state.keyboard[size_t(key_jump)])
			vel_[1]-= c_jump_vel;
	}

	// Calculate acceleration vector.
	if(input_state.keyboard[size_t(key_left)])
		accel_vec[0]+= g_fixed16_one;
	if(input_state.keyboard[size_t(key_right)])
		accel_vec[0]-= g_fixed16_one;

	const bool use_up_down_acceleration= false; // TODO - make true for fly/swim.
	if(use_up_down_acceleration)
	{
		if(input_state.keyboard[size_t(key_down)])
			accel_vec[1]+= g_fixed16_one;
		if(input_state.keyboard[size_t(key_up)])
			accel_vec[1]-= g_fixed16_one;
	}

	{ // Normalize acceleration vector.
		const fixed16_t len= Fixed16VecLen(accel_vec);
		if(len > 0)
		{
			accel_vec[0]= Fixed16Div(accel_vec[0], len);
			accel_vec[1]= Fixed16Div(accel_vec[1], len);
		}
	}

	{ // Accelerate, but limit result projection of current velocity to wish direction.
		const fixed16_t vel_proj= Fixed16VecDot(vel_, accel_vec); // Length of projection of current speed on wish direction.
		if(vel_proj < c_max_vel)
		{
			const fixed16_t current_acceleration= on_ground_ ? c_ground_acceleration : c_air_acceleration;
			const fixed16_t add_speed= std::min(c_max_vel - vel_proj, current_acceleration);
			vel_[0]+= Fixed16Mul(add_speed, accel_vec[0]);
			vel_[1]+= Fixed16Mul(add_speed, accel_vec[1]);
		}
	}

	// Apply friction.
	const fixed16_t vel_modulo= Fixed16VecLen(vel_);
	if(vel_modulo > 0)
	{
		const fixed16_t current_friction= on_ground_ ? c_ground_friction : c_air_friction;
		const fixed16_t sub_speed= std::min(vel_modulo, current_friction);
		const fixed16vec2_t prev_vel{vel_[0], vel_[1]};
		for(uint32_t i= 0; i < 2; ++i)
			vel_[i]-= Fixed16Div(Fixed16Mul(vel_[i], sub_speed), vel_modulo);

		// Fix calculation errors - make velosity zero in case of new velosity points in another direction.
		if( Fixed16VecDot(vel_, prev_vel) < 0)
			vel_[0]= vel_[1]= 0;
	}
	else
		vel_[0]= vel_[1]= 0; // Zero small speed value.

	// Apply gravity after friction to avoid reset of small gravity-based velocity change.
	vel_[1]+= c_gravity;
	if(vel_[1] > c_terminal_vel)
		vel_[1]= c_terminal_vel;

	for(uint32_t i= 0; i < 2; ++i)
		pos_[i]+= vel_[i];

}

Player::ShootRequestKind Player::Shoot(const InputState& input_state)
{
	if(input_state.mouse[size_t(SystemEventTypes::ButtonCode::Left)] &&
		current_tick_ - last_shoot_tick_ >= 20 &&
		ammo_ > 0)
	{
		--ammo_;
		last_shoot_tick_= current_tick_;
		return ShootRequestKind::Machinegun;
	}

	return ShootRequestKind::None;
}

} // namespace Armed
