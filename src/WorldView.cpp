#include "WorldView.hpp"
#include "Assert.hpp"
#include "Models/Models.hpp"

namespace Armed
{

TransformMatrix MatrixMul(const TransformMatrix& l, const TransformMatrix& r)
{
	TransformMatrix res{};
	res.scale[0] = Fixed16Mul(l.scale[0], r.scale[0]);
	res.scale[1] = Fixed16Mul(l.scale[1], r.scale[1]);
	res.shift[0]= Fixed16Mul(l.shift[0], r.scale[0]) + r.shift[0];
	res.shift[1]= Fixed16Mul(l.shift[1], r.scale[1]) + r.shift[1];
	return res;
}

fixed16_t TransformX(const TransformMatrix& mat, const fixed16_t x)
{
	return Fixed16Mul(mat.scale[0], x) + mat.shift[0];
}

fixed16_t TransformY(const TransformMatrix& mat, const fixed16_t y)
{
	return Fixed16Mul(mat.scale[1], y) + mat.shift[1];
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
		auto& dst= reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + uint32_t(surface.pitch) * y)[x];
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

	for(const World::Projectile& projectile : world_.GetProjectiles())
		DrawProjectile(mat, surface, projectile);
}

TransformMatrix WorldView::CalculateViewTransformMatrix(const SDL_Surface& surface)
{
	const Player& player= world_.GetPlayer();

	TransformMatrix cam_shift{}, scale{}, screen_shift{};

	cam_shift.scale[0]= cam_shift.scale[1] = g_fixed16_one;
	cam_shift.shift[0]= -player.GetPos()[0];
	cam_shift.shift[1]= -player.GetPos()[1];

	scale.scale[0]= scale.scale[1]= IntToFixed16(24);
	scale.shift[0]= scale.shift[1]= 0;

	screen_shift.scale[0]= screen_shift.scale[1]= g_fixed16_one;
	screen_shift.shift[0]= IntToFixed16(surface.w) / 2;
	screen_shift.shift[1]= IntToFixed16(surface.h) / 2;

	return MatrixMul(MatrixMul(cam_shift, scale), screen_shift);
}

void WorldView::DrawTile(const TransformMatrix& view_mat, const SDL_Surface& surface, const uint32_t tile_x, const uint32_t tile_y, const TileId tile)
{
	ARMED_UNUSED(tile);

	TransformMatrix tile_mat;
	tile_mat.scale[0]= tile_mat.scale[1] = g_fixed16_one;
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
	player_mat.scale[0]= player_mat.scale[1] = g_fixed16_one;
	player_mat.shift[0]= player.GetPos()[0];
	player_mat.shift[1]= player.GetPos()[1];

	const TransformMatrix result_mat= MatrixMul(player_mat, view_mat);

	DrawModel(result_mat, surface, Models::player);
}

void WorldView::DrawMonster(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::Monster& monster)
{
	TransformMatrix monster_mat;
	monster_mat.scale[0]= monster_mat.scale[1] = g_fixed16_one;
	monster_mat.shift[0]= monster.pos[0];
	monster_mat.shift[1]= monster.pos[1];

	if(monster.move_dir <0)
		monster_mat.scale[0]*= -1;

	const TransformMatrix result_mat= MatrixMul(monster_mat, view_mat);

	DrawModel(result_mat, surface, Models::monster_biter);
}

void WorldView::DrawProjectile(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::Projectile& projectile)
{
	const fixed16_t c_radius= g_fixed16_one / 8;
	const fixed16vec2_t center_projected{TransformX(view_mat, projectile.pos[0]), TransformY(view_mat, projectile.pos[1])};

	const fixed16_t x_plus_edge_projected= TransformX(view_mat, projectile.pos[0] + c_radius);
	const fixed16_t radius_projected= x_plus_edge_projected - center_projected[0];

	FillCircle(surface, center_projected, radius_projected, ColorRGB(0, 255, 255));
}

void WorldView::DrawModel(const TransformMatrix& mat, const SDL_Surface& surface, const Model& model)
{
	for(const ModelTrapezoid& trapeziod : model)
	{
		fixed16_t y_start= TransformY(mat, trapeziod.sides[0].y);
		fixed16_t y_end= TransformY(mat, trapeziod.sides[1].y);
		fixed16_t x_start_0= TransformX(mat, trapeziod.sides[0].x[0]);
		fixed16_t x_start_1= TransformX(mat, trapeziod.sides[0].x[1]);
		fixed16_t x_end_0= TransformX(mat, trapeziod.sides[1].x[0]);
		fixed16_t x_end_1= TransformX(mat, trapeziod.sides[1].x[1]);
		if(mat.scale[0] < 0)
		{
			std::swap(x_start_0, x_start_1);
			std::swap(x_end_0, x_end_1);
		}
		if(mat.scale[1] < 0)
			std::swap(y_start, y_end);

		FillTrapezoid(
			surface,
			y_start, y_end,
			x_start_0, x_start_1,
			x_end_0, x_end_1, trapeziod.color);
	}
}

void WorldView::FillRectangle(const SDL_Surface& surface, const int32_t min_x, const int32_t min_y, const int32_t max_x, const int32_t max_y, color_t color)
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
		auto& dst= reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y)[x];
		dst= color;
	}
}

void WorldView::FillTrapezoid(
	const SDL_Surface& surface,
	const fixed16_t y_start, const fixed16_t y_end,
	const fixed16_t x_start_0, const fixed16_t x_start_1,
	const fixed16_t x_end_0, const fixed16_t x_end_1,
	const color_t color)
{
	const int32_t surface_width= surface.w;
	const int32_t surface_height= surface.h;

	const fixed16_t dy= y_end - y_start;
	if( dy < g_fixed16_one)
	{
		// Do not calculate dx/dy for small "dy" to avoid problems with precision.
		const int32_t row= std::max(0, Fixed16RoundToInt(y_start));
		const int32_t row_end= std::min(Fixed16RoundToInt(y_end), surface_height);
		if(row < row_end)
		{
			const fixed16_t y_change= IntToFixed16(row) + (g_fixed16_one/2) - y_start;
			const fixed16_t x0= x_start_0 + Fixed16MulDiv(x_end_0 - x_start_0, y_change, dy);
			const fixed16_t x1= x_start_1 + Fixed16MulDiv(x_end_1 - x_start_1, y_change, dy);

			auto dst= reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * row);
			for(int32_t column= std::max(0, Fixed16RoundToInt(x0)), column_end= std::min(Fixed16RoundToInt(x1), surface_width); column < column_end; ++column)
				dst[column]= color;
		}
	}
	else
	{
		const fixed16_t d_x0= Fixed16Div(x_end_0 - x_start_0, dy);
		const fixed16_t d_x1= Fixed16Div(x_end_1 - x_start_1, dy);
		for(int32_t row= std::max(0, Fixed16RoundToInt(y_start)), row_end= std::min(Fixed16RoundToInt(y_end), surface_height); row < row_end; ++row)
		{
			const fixed16_t y_change= IntToFixed16(row) + (g_fixed16_one/2) - y_start;
			const fixed16_t x0= x_start_0 + Fixed16Mul(y_change, d_x0);
			const fixed16_t x1= x_start_1 + Fixed16Mul(y_change, d_x1);

			auto dst= reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * row);
			for(int32_t column= std::max(0, Fixed16RoundToInt(x0)), column_end= std::min(Fixed16RoundToInt(x1), surface_width); column < column_end; ++column)
				dst[column]= color;
		}
	}
}

void WorldView::FillCircle(const SDL_Surface& surface, const fixed16vec2_t& center, const fixed16_t radius, const color_t color)
{
	const int32_t x_start= std::max(0, Fixed16RoundToInt(center[0] - radius));
	const int32_t x_end= std::min(Fixed16RoundToInt(center[0] + radius), surface.w);
	const int32_t y_start= std::max(0, Fixed16RoundToInt(center[1] - radius));
	const int32_t y_end= std::min(Fixed16RoundToInt(center[1] + radius), surface.h);

	const fixed16_t square_radius= Fixed16Mul(radius, radius);

	for(int32_t y= y_start; y < y_end; ++y)
	{
		const fixed16_t dy = IntToFixed16(y) + g_fixed16_one / 2 - center[1];
		const fixed16_t dy2= Fixed16Mul(dy, dy);
		auto dst= reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y);
		for(int32_t x= x_start; x < x_end; ++x)
		{
			const fixed16_t dx = IntToFixed16(x) + g_fixed16_one / 2 - center[0];
			const fixed16_t dx2= Fixed16Mul(dx, dx);
			const fixed16_t square_dist= dx2 + dy2;
			if(square_dist <= square_radius)
				dst[x]= color;
		}
	}
}

} // namespace Armed
