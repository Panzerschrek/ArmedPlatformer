#pragma once
#include "Player.hpp"
#include "TilesMap.hpp"

namespace Armed
{

class World
{
public:
	explicit World(const char* tiles_map_data);

	void ProcessSystemEvent(const SystemEvent& event);
	void Update(const InputState& input_state, float dt_s);

	const TilesMap& GetMap() const { return map_; }
	const Player& GetPlayer() const { return player_; }

private:
	TilesMap map_;
	Player player_;
};

} // namespace Armed
