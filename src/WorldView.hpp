#pragma once
#include "SystemWindow.hpp"
#include "World.hpp"

namespace Armed
{

class WorldView
{
public:
	WorldView(const World& world, SystemWindow& system_window);

	void Draw();

private:
	const World& world_;
	SystemWindow& system_window_;
};

} // namespace Armed
