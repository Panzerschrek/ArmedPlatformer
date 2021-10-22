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

	void Push(const fixed16vec2_t& push_vec);
	void SetOnGround(const bool on_ground){ on_ground_= on_ground; }

private:
	fixed16vec2_t pos_;
	fixed16vec2_t vel_;
	bool on_ground_= false;
};

} // namespace Armed
