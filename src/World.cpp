#include "World.hpp"
#include "Assert.hpp"

namespace Armed
{

World::World(const char* tiles_map_data)
	: map_(tiles_map_data)
	, player_(0, 0)
{
}

void World::Tick(const InputState& input_state)
{
	player_.Tick(input_state);
}


} // namespace Armed
