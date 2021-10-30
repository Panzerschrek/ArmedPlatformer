#pragma once
#include "Player.hpp"
#include "Rand.hpp"
#include "TilesMap.hpp"

namespace Armed
{

struct MapDescription
{
	const char* tiles_map_data= nullptr;
	fixed16vec2_t player_spawn{};
};

class World
{
public:
	static constexpr const uint32_t c_update_frequency= 120;

	using TickT= uint32_t;

	struct Monster
	{
		uint32_t spawn_tile_pos[2]{};
		fixed16vec2_t pos{};
		int32_t move_dir= 0; // -1, 0, +1
		TickT last_attack_tick= 0;
		int32_t health= 0;
	};

	struct Projectile
	{
		enum class OwnerKind{ Player, Monster };

		OwnerKind owner_kind= OwnerKind::Player;
		fixed16vec2_t pos{};
		fixed16vec2_t vel{};
	};

public:
	explicit World(const MapDescription& map_description);

	// Update world (single fixed period step).
	void Tick(const InputState& input_state, const fixed16vec2_t& aim_vec);

	const TilesMap& GetMap() const { return map_; }
	const Player& GetPlayer() const { return player_; }
	const std::vector<Monster>& GetMonsters() const { return monsters_; }
	const std::vector<Projectile>& GetProjectiles() const { return projectiles_; }

private:
	void ProcessPlayerPhysics();
	void ProcessShootRequest(Player::ShootRequestKind shoot_request);
	void MoveMonsters();
	void MoveMonster(Monster& monster);
	void MoveProjectiles();
	bool MoveProjectile(Projectile& projectile); // Returns true if should keep alive.

private:
	Rand rand_; // TODO - initialize it with really-random seed
	TilesMap map_;
	Player player_;
	std::vector<Monster> monsters_;
	std::vector<Projectile> projectiles_;
	TickT current_tick_ = c_update_frequency * 100;
};

} // namespace Armed
