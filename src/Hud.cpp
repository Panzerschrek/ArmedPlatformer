#include "Hud.hpp"
#include "Color.hpp"

namespace Armed
{

Hud::Hud(const World& world, SystemWindow& system_window)
	: world_(world), system_window_(system_window)
{
}

void Hud::Draw()
{
	ARMED_UNUSED(world_);

	const int32_t c_hud_heigth= 64;

	const SDL_Surface& surface= system_window_.GetSurface();
	const int32_t surface_width= surface.w;
	const int32_t surface_height= surface.h;

	// Make darker HUD bar area.
	for(int32_t y= surface_height - c_hud_heigth; y < surface_height; ++y)
	{
		auto dst= reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y);
		for(int32_t x= 0; x < surface_width; ++x)
			dst[x]= (dst[x] & 0xFEFEFEFE) >> 1;
	}
}

} // namespace Armed
