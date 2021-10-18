#include "World.hpp"
#include "Assert.hpp"

namespace Armed
{

World::World(const char* tiles_map_data)
	: map_(tiles_map_data)
	, player_(0, 0)
{
}

void World::Update(const InputState& input_state, float dt_s)
{
	ARMED_UNUSED(dt_s); // TODO - use fixed updates frequency regardless of FPS.

	player_.Tick(input_state);
}


} // namespace Armed
