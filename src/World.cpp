#include "World.hpp"
#include "Assert.hpp"

namespace Armed
{

World::World(const char* tiles_map_data)
	: map_(tiles_map_data)
{
}

void World::Update(const float dt_s)
{
	ARMED_UNUSED(dt_s);
}

} // namespace Armed
