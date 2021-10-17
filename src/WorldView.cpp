#include "WorldView.hpp"
#include "Assert.hpp"

namespace Armed
{

WorldView::WorldView(const World& world, SystemWindow& system_window)
	: world_(world), system_window_(system_window)
{
}

void WorldView::Draw()
{
	ARMED_UNUSED(world_);

	using pixel_t= uint32_t;

	const SDL_Surface& surface= system_window_.GetSurface();

	for(uint32_t y= 0; y < uint32_t(surface.h); ++y)
	for(uint32_t x= 0; x < uint32_t(surface.w); ++x)
	{
		auto& dst= reinterpret_cast<pixel_t*>(static_cast<char*>(surface.pixels) + uint32_t(surface.pitch) * y)[x];
		dst= uint32_t(x) * 255u / uint32_t(surface.w) + ((uint32_t(y) * 255u / uint32_t(surface.h)) << 8);
	}
}

} // namespace Armed
