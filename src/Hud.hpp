#pragma once
#include "SystemWindow.hpp"
#include "World.hpp"

namespace Armed
{

class Hud
{
public:
	Hud(const World& world, SystemWindow& system_window);

	void Draw();

private:
	const World& world_;
	SystemWindow& system_window_;
};

} // namespace Armed
