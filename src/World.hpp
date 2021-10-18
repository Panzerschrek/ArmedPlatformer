#pragma once
#include "SystemEvent.hpp"
#include "TilesMap.hpp"

namespace Armed
{

class World
{
public:
	explicit World(const char* tiles_map_data);

	void ProcessSystemEvent(const SystemEvent& event);
	void Update(float dt_s);

private:
	TilesMap map_;
};

} // namespace Armed
