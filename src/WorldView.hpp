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
	void DrawTile(const SDL_Surface& surface, uint32_t tile_x, uint32_t tile_y, TileId tile);

private:
	const World& world_;
	SystemWindow& system_window_;
};

} // namespace Armed
