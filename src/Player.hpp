#pragma once
#include "SystemEvent.hpp"
#include "Fixed.hpp"

namespace Armed
{

class Player
{
public:
	enum class ShootRequestKind
	{
		None,
		Machinegun,
	};

	static constexpr int32_t c_max_health= 100;

	Player(fixed16_t pos_x, fixed16_t pos_y);

	ShootRequestKind Tick(const InputState& input_state, const fixed16vec2_t& aim_vec);

	const fixed16vec2_t& GetPos() const { return pos_; }
	const fixed16vec2_t& GetAimNormal() const { return aim_vec_; }
	int32_t GetHealth() const { return health_; }
	uint32_t GetAmmo() const { return ammo_; }

	void Push(const fixed16vec2_t& push_vec);
	void AccelerateByMovingPlatform(const fixed16vec2_t& target_vel);
	void SetOnGround(const bool on_ground){ on_ground_= on_ground; }
	void Hit(int32_t damage);

private:
	void Move(const InputState& input_state);
	ShootRequestKind Shoot(const InputState& input_state);

private:
	uint32_t current_tick_= 100000;
	fixed16vec2_t pos_;
	fixed16vec2_t vel_{};
	fixed16vec2_t aim_vec_{};
	bool on_ground_= false;
	int32_t health_= c_max_health;

	uint32_t ammo_= 48;
	uint32_t last_shoot_tick_= 0;
};

} // namespace Armed
