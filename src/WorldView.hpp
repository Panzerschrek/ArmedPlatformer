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
	struct CameraParams
	{
		int32_t pos[2];
		int32_t tile_size;
	};

private:
	CameraParams CalculateCameraParams();
	void DrawTile(const CameraParams& camera, const SDL_Surface& surface, uint32_t tile_x, uint32_t tile_y, TileId tile);

private:
	const World& world_;
	SystemWindow& system_window_;
};

} // namespace Armed
