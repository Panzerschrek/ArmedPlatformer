#include "WorldView.hpp"
#include "Assert.hpp"

namespace Armed
{

namespace
{

using pixel_t= uint32_t;

} // namespace

WorldView::WorldView(const World& world, SystemWindow& system_window)
	: world_(world), system_window_(system_window)
{
}

void WorldView::Draw()
{
	const SDL_Surface& surface= system_window_.GetSurface();

	for(uint32_t y= 0; y < uint32_t(surface.h); ++y)
	for(uint32_t x= 0; x < uint32_t(surface.w); ++x)
	{
		auto& dst= reinterpret_cast<pixel_t*>(static_cast<char*>(surface.pixels) + uint32_t(surface.pitch) * y)[x];
		dst= uint32_t(x) * 255u / uint32_t(surface.w) + ((uint32_t(y) * 255u / uint32_t(surface.h)) << 8);
	}

	const TilesMap& tiles_map= world_.GetMap();
	const CameraParams camera_params= CalculateCameraParams();

	for(uint32_t y= 0; y < tiles_map.GetSizeY(); ++y)
	for(uint32_t x= 0; x < tiles_map.GetSizeX(); ++x)
	{
		const TileId tile= tiles_map.GetTile(x, y);

		if(tile != TileId::Air)
			DrawTile(camera_params, surface, x, y, tile);
	}
}

WorldView::CameraParams WorldView::CalculateCameraParams()
{
	CameraParams res{};

	const Player& player= world_.GetPlayer();

	res.pos[0]= Fixed16FloorToInt(player.GetPos()[0]);
	res.pos[1]= Fixed16FloorToInt(player.GetPos()[1]);
	res.tile_size= 24;
	return res;
}

void WorldView::DrawTile(const CameraParams& camera, const SDL_Surface& surface, const uint32_t tile_x, const uint32_t tile_y, const TileId tile)
{
	ARMED_UNUSED(tile);

	const int32_t projected_min_x= (int32_t(tile_x) - camera.pos[0]) * camera.tile_size;
	const int32_t projected_min_y= (int32_t(tile_y) - camera.pos[1]) * camera.tile_size;
	const int32_t projected_max_x= projected_min_x + camera.tile_size;
	const int32_t projected_max_y= projected_min_y + camera.tile_size;

	if(projected_min_x >= surface.w ||
		projected_min_y >= surface.h ||
		projected_max_x <= 0 ||
		projected_max_y <= 0)
		return;

	const int32_t min_x_clamped= std::max(0, projected_min_x);
	const int32_t min_y_clamped= std::max(0, projected_min_y);
	const int32_t max_x_clamped= std::min(projected_max_x, surface.w);
	const int32_t max_y_clamped= std::min(projected_max_y, surface.h);

	for(int32_t y= min_y_clamped; y < max_y_clamped; ++y)
	for(int32_t x= min_x_clamped; x < max_x_clamped; ++x)
	{
		auto& dst= reinterpret_cast<pixel_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y)[x];
		dst= 0xFF00FF00;
	}
}

} // namespace Armed
