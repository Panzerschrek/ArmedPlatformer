#include "WorldView.hpp"
#include "Assert.hpp"

namespace Armed
{

TransformMatrix MatrixMul(const TransformMatrix& l, const TransformMatrix& r)
{
	TransformMatrix res{};
	res.scale = Fixed16Mul(l.scale, r.scale);
	res.shift[0]= Fixed16Mul(l.shift[0], r.scale) + r.shift[0];
	res.shift[1]= Fixed16Mul(l.shift[1], r.scale) + r.shift[1];
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
	const TransformMatrix mat= CalculateViewTransformMatrix(surface);

	for(uint32_t y= 0; y < tiles_map.GetSizeY(); ++y)
	for(uint32_t x= 0; x < tiles_map.GetSizeX(); ++x)
	{
		const TileId tile= tiles_map.GetTile(x, y);

		if(tile != TileId::Air)
			DrawTile(mat, surface, x, y, tile);
	}

	DrawPlayer(mat, surface);

	for(const World::Monster& monster : world_.GetMonsters())
		DrawMonster(mat, surface, monster);
}

TransformMatrix WorldView::CalculateViewTransformMatrix(const SDL_Surface& surface)
{
	const Player& player= world_.GetPlayer();

	TransformMatrix cam_shift{}, scale{}, screen_shift{};

	cam_shift.scale = g_fixed16_one;
	cam_shift.shift[0]= -player.GetPos()[0];
	cam_shift.shift[1]= -player.GetPos()[1];

	scale.scale= IntToFixed16(24);
	scale.shift[0]= scale.shift[1]= 0;

	screen_shift.scale= g_fixed16_one;
	screen_shift.shift[0]= IntToFixed16(surface.w) / 2;
	screen_shift.shift[1]= IntToFixed16(surface.h) / 2;

	return MatrixMul(MatrixMul(cam_shift, scale), screen_shift);
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

	FillRectangle(surface, projected_min_x, projected_min_y, projected_max_x, projected_max_y, 0xFF00FF00);
}

void WorldView::DrawPlayer(const TransformMatrix& view_mat, const SDL_Surface& surface)
{
	const Player& player= world_.GetPlayer();

	TransformMatrix player_mat;
	player_mat.scale = g_fixed16_one;
	player_mat.shift[0]= player.GetPos()[0];
	player_mat.shift[1]= player.GetPos()[1];

	const TransformMatrix result_mat= MatrixMul(player_mat, view_mat);

	const fixed16_t width= g_fixed16_one / 2;
	const fixed16_t height = g_fixed16_one * 3 / 4;
	const fixed16_t min_x= -  width / 2;
	const fixed16_t max_x= +  width / 2;
	const fixed16_t min_y= - height / 2;
	const fixed16_t max_y= + height / 2;

	const int32_t projected_min_x= Fixed16RoundToInt(TransformX(result_mat, min_x));
	const int32_t projected_min_y= Fixed16RoundToInt(TransformY(result_mat, min_y));
	const int32_t projected_max_x= Fixed16RoundToInt(TransformX(result_mat, max_x));
	const int32_t projected_max_y= Fixed16RoundToInt(TransformY(result_mat, max_y));

	FillRectangle(surface, projected_min_x, projected_min_y, projected_max_x, projected_max_y, 0x00FF00FF);
}

void WorldView::DrawMonster(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::Monster& monster)
{
	TransformMatrix monster_mat;
	monster_mat.scale = g_fixed16_one;
	monster_mat.shift[0]= monster.pos[0];
	monster_mat.shift[1]= monster.pos[1];

	const TransformMatrix result_mat= MatrixMul(monster_mat, view_mat);

	const fixed16_t width= g_fixed16_one * 5 / 8;
	const fixed16_t height = g_fixed16_one * 6 / 8;
	const fixed16_t min_x= -  width / 2;
	const fixed16_t max_x= +  width / 2;
	const fixed16_t min_y= - height / 2;
	const fixed16_t max_y= + height / 2;

	const int32_t projected_min_x= Fixed16RoundToInt(TransformX(result_mat, min_x));
	const int32_t projected_min_y= Fixed16RoundToInt(TransformY(result_mat, min_y));
	const int32_t projected_max_x= Fixed16RoundToInt(TransformX(result_mat, max_x));
	const int32_t projected_max_y= Fixed16RoundToInt(TransformY(result_mat, max_y));

	FillRectangle(surface, projected_min_x, projected_min_y, projected_max_x, projected_max_y, 0x0000FFFF);
}

void WorldView::FillRectangle(const SDL_Surface& surface, const int32_t min_x, const int32_t min_y, const int32_t max_x, const int32_t max_y, pixel_t color)
{
	if(min_x >= surface.w ||
		min_y >= surface.h ||
		max_x <= 0 ||
		max_y <= 0)
		return;

	const int32_t min_x_clamped= std::max(0, min_x);
	const int32_t min_y_clamped= std::max(0, min_y);
	const int32_t max_x_clamped= std::min(max_x, surface.w);
	const int32_t max_y_clamped= std::min(max_y, surface.h);

	for(int32_t y= min_y_clamped; y < max_y_clamped; ++y)
	for(int32_t x= min_x_clamped; x < max_x_clamped; ++x)
	{
		auto& dst= reinterpret_cast<pixel_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y)[x];
		dst= color;
	}
}

} // namespace Armed
