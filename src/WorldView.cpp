#include "WorldView.hpp"
#include "Assert.hpp"

namespace Armed
{

namespace
{

using pixel_t= uint32_t;

} // namespace

TransformMatrix MatrixMul(const TransformMatrix& l, const TransformMatrix& r)
{
	TransformMatrix res{};
	res.scale = Fixed16Mul(l.scale, r.scale);
	res.shift[0]= Fixed16Mul(l.shift[0], r.scale) + r.shift[0];
	res.shift[1]= Fixed16Mul(l.shift[1], r.scale)+ r.shift[1];
	return res;
}

fixed16_t TransformX(const TransformMatrix& mat, const fixed16_t x)
{
	return Fixed16Mul(mat.scale, x) + mat.shift[0];
}

fixed16_t TransformY(const TransformMatrix& mat, const fixed16_t y)
{
	return Fixed16Mul(mat.scale, y) + mat.shift[1];
}

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
	const TransformMatrix mat= CalculateViewTransformMatrix();

	for(uint32_t y= 0; y < tiles_map.GetSizeY(); ++y)
	for(uint32_t x= 0; x < tiles_map.GetSizeX(); ++x)
	{
		const TileId tile= tiles_map.GetTile(x, y);

		if(tile != TileId::Air)
			DrawTile(mat, surface, x, y, tile);
	}
}

TransformMatrix WorldView::CalculateViewTransformMatrix()
{
	const Player& player= world_.GetPlayer();

	TransformMatrix res{};
	res.scale = IntToFixed16(24);
	res.shift[0]= -player.GetPos()[0];
	res.shift[1]= -player.GetPos()[1];
	return res;
}

void WorldView::DrawTile(const TransformMatrix& view_mat, const SDL_Surface& surface, const uint32_t tile_x, const uint32_t tile_y, const TileId tile)
{
	ARMED_UNUSED(tile);

	TransformMatrix tile_mat;
	tile_mat.scale = g_fixed16_one;
	tile_mat.shift[0]= IntToFixed16(int32_t(tile_x));
	tile_mat.shift[1]= IntToFixed16(int32_t(tile_y));

	const TransformMatrix result_mat= MatrixMul(tile_mat, view_mat);

	const fixed16_t min_x= 0;
	const fixed16_t max_x= g_fixed16_one;
	const fixed16_t min_y= 0;
	const fixed16_t max_y= g_fixed16_one;

	const int32_t projected_min_x= Fixed16RoundToInt(TransformX(result_mat, min_x));
	const int32_t projected_min_y= Fixed16RoundToInt(TransformY(result_mat, min_y));
	const int32_t projected_max_x= Fixed16RoundToInt(TransformX(result_mat, max_x));
	const int32_t projected_max_y= Fixed16RoundToInt(TransformY(result_mat, max_y));

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
