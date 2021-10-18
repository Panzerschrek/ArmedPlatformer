#include "Player.hpp"

namespace Armed
{

Player::Player(const fixed16_t pos_x, const fixed16_t pos_y)
	: pos_{pos_x, pos_y}
{
}

void Player::Tick(const InputState& input_state)
{
	using KeyCode= SystemEventTypes::KeyCode;
	const auto key_left = KeyCode::D;
	const auto key_right = KeyCode::A;
	const auto key_up = KeyCode::W;
	const auto key_down = KeyCode::S;

	const fixed16_t speed= g_fixed16_one / 64;

	if(input_state.keyboard[size_t(key_left)])
		pos_[0]+= speed;
	if(input_state.keyboard[size_t(key_right)])
		pos_[0]-= speed;
	if(input_state.keyboard[size_t(key_down)])
		pos_[1]+= speed;
	if(input_state.keyboard[size_t(key_up)])
		pos_[1]-= speed;
}

} // namespace Armed
