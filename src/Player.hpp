#pragma once
#include "Fixed.hpp"
#include "Assert.hpp"
#include "SystemEvent.hpp"

namespace Armed
{

class Player
{
public:
	enum class ShootRequestKind
	{
		None,
		Machinegun,
		GrenadeLauncher,
	};

	static constexpr int32_t c_max_health= 100;
	static constexpr uint32_t c_max_ammo[4]{48, 16, 0, 0};

	Player(fixed16_t pos_x, fixed16_t pos_y);

	ShootRequestKind Tick(const InputState& input_state, const fixed16vec2_t& aim_vec);

	const fixed16vec2_t& GetPos() const { return pos_; }
	const fixed16vec2_t& GetAimNormal() const { return aim_vec_; }
	int32_t GetHealth() const { return health_; }
	uint32_t GetAmmo(const size_t index) const { ARMED_ASSERT(index < std::size(ammo_)); return ammo_[index]; }
	size_t GetCurrentWeapon() const { return current_weapon_; }

	void Push(const fixed16vec2_t& push_vec);
	void AccelerateByMovingPlatform(const fixed16vec2_t& target_vel);
	void SetOnGround(const bool on_ground){ on_ground_= on_ground; }
	void SetInLiquid(const bool in_liquid){ in_liquid_= in_liquid; }
	void Hit(int32_t damage);
	void GiveAmmo(size_t index, uint32_t count);
	void GiveKey(const size_t key_index){ ARMED_ASSERT(key_index < std::size(keys_)); keys_[key_index]= true; }
	bool HasKey(const size_t key_index) const {ARMED_ASSERT(key_index < std::size(keys_)); return keys_[key_index]; }

private:
	void Move(const InputState& input_state);
	ShootRequestKind Shoot(const InputState& input_state);

private:
	uint32_t current_tick_= 100000;
	fixed16vec2_t pos_;
	fixed16vec2_t vel_{};
	fixed16vec2_t aim_vec_{};
	bool on_ground_= false;
	bool in_liquid_= false;
	size_t current_weapon_= 0;
	uint32_t ammo_[4]= {48, 0, 0, 0};
	bool keys_[4]= {false, false, false, false};
	int32_t health_= c_max_health;

	uint32_t last_shoot_tick_= 0;
};

} // namespace Armed
