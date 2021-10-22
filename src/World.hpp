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

public:
	explicit World(const MapDescription& map_description);

	// Update world (single fixed period step).
	void Tick(const InputState& input_state);

	const TilesMap& GetMap() const { return map_; }
	const Player& GetPlayer() const { return player_; }

private:
	TilesMap map_;
	Player player_;
};

} // namespace Armed
