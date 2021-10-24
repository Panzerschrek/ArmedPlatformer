#pragma once
#include "Player.hpp"
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
	static constexpr const uint64_t c_update_frequency= 120;

	struct Monster
	{
		uint32_t spawn_tile_pos[2]{};
		fixed16vec2_t pos{};
		int32_t move_dir= 0; // -1, 0, +1
	};

public:
	explicit World(const MapDescription& map_description);

	// Update world (single fixed period step).
	void Tick(const InputState& input_state);

	const TilesMap& GetMap() const { return map_; }
	const Player& GetPlayer() const { return player_; }
	const std::vector<Monster>& GetMonsters() const { return monsters_; }

private:
	void ProcessPlayerPhysics(const InputState& input_state);
	void MoveMonsters();
	void MoveMonster(Monster& monster);

private:
	TilesMap map_;
	Player player_;
	std::vector<Monster> monsters_;
};

} // namespace Armed
