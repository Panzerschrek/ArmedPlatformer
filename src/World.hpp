#pragma once
#include "Player.hpp"
#include "TilesMap.hpp"

namespace Armed
{

class World
{
public:
	static constexpr const uint64_t c_update_frequency= 120;

public:
	explicit World(const char* tiles_map_data);

	// Update world (single fixed period step).
	void Tick(const InputState& input_state);

	const TilesMap& GetMap() const { return map_; }
	const Player& GetPlayer() const { return player_; }

private:
	TilesMap map_;
	Player player_;
};

} // namespace Armed
