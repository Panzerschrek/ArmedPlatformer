#pragma once
#include "SystemEvent.hpp"
#include "Fixed.hpp"

namespace Armed
{

class Player
{
public:
	Player(fixed16_t pos_x, fixed16_t pos_y);

	void Tick(const InputState& input_state);

	const fixed16vec2_t& GetPos() const { return pos_; }

	void Push(const fixed16vec2_t& dir);

private:
	fixed16vec2_t pos_;
	fixed16vec2_t vel_;
};

} // namespace Armed
