#pragma once
#include "SystemEvent.hpp"

namespace Armed
{

class World
{
public:
	void ProcessSystemEvent(const SystemEvent& event);
	void Update(float dt_s);

private:
};

} // namespace Armed
