#include "WorldView.hpp"
#include "Assert.hpp"
#include "Models/Models.hpp"

namespace Armed
{

namespace
{

const Model& GetModelForMonster(const MonsterId id)
{
	switch(id)
	{
	case MonsterId::Biter: return Models::monster_biter;
	case MonsterId::Gunner: return Models::monster_gunner;
	case MonsterId::Bomber: return Models::monster_bomber;
	};

	ARMED_ASSERT(false);
	return Models::monster_biter;
}

const Model& GetModelForPowerup(const PowerUpId id)
{
	switch(id)
	{
	case PowerUpId::SmallHealth:
		return Models::small_health;
	case PowerUpId::LargeHealth:
	case PowerUpId::Health2:
	case PowerUpId::Health3:
		return Models::large_health;
	case PowerUpId::Ammo0:
		return Models::ammo0;
	case PowerUpId::Ammo1:
	case PowerUpId::Ammo2:
	case PowerUpId::Ammo3:
		return Models::ammo1;
	case PowerUpId::Key0: return Models::key0;
	case PowerUpId::Key1: return Models::key1;
	case PowerUpId::Key2: return Models::key2;
	case PowerUpId::Key3: return Models::key3;
	};

	ARMED_ASSERT(false);
	return Models::small_health;
}

struct ProjectileModel
{
	fixed16_t radius;
	color_t color;
};

ProjectileModel GetProjectileModel(const World::Projectile::Kind kind)
{
	using Kind= World::Projectile::Kind;
	switch(kind)
	{
	case Kind::Bullet: return {g_fixed16_one / 10, ColorRGB(200, 200, 200)};
	case Kind::Grenade: return {g_fixed16_one / 6, ColorRGB(220, 120, 64)};
	case Kind::Bomb: return {g_fixed16_one / 4, ColorRGB(32, 64, 64)};
	}

	ARMED_ASSERT(false);
	return {g_fixed16_one, ColorRGB(255, 0, 255)};
}

} // namespace

WorldView::WorldView(const World& world, const Camera& camera, SystemWindow& system_window)
	: world_(world), camera_(camera), system_window_(system_window)
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

	const Player& player= world_.GetPlayer();
	const fixed16vec2_t player_pos= player.GetPos();

	const TilesMap& tiles_map= world_.GetMap();
	const TransformMatrix mat= camera_.CalculateMatrix();

	for(uint32_t y= 0; y < tiles_map.GetSizeY(); ++y)
	for(uint32_t x= 0; x < tiles_map.GetSizeX(); ++x)
	{
		const TileId tile= tiles_map.GetTile(x, y);

		// Draw key model atop of door tiles.
		if(tile >= TileId::KeyDoor0 && tile <= TileId::KeyDoor3)
		{
			const fixed16vec2_t key_sign_pos= {IntToFixed16(int32_t(x)) + g_fixed16_one / 2, IntToFixed16(int32_t(y)) + g_fixed16_one / 3};

			const bool is_nearby= std::abs(player_pos[0] - key_sign_pos[0]) + std::abs(player_pos[1] - key_sign_pos[1]) <= g_fixed16_one * 2;
			if(!(is_nearby && player.HasKey(size_t(tile) - size_t(TileId::KeyDoor0))))
			{
				DrawTile(mat, surface, x, y, tile);

				TransformMatrix key_mat;
				key_mat.scale[0]= key_mat.scale[1] = g_fixed16_one * 2 / 3;
				key_mat.shift= key_sign_pos;

				DrawModel(
					MatrixMul(key_mat, mat),
					surface,
					GetModelForPowerup(PowerUpId(size_t(tile) - size_t(TileId::KeyDoor0) + size_t(PowerUpId::Key0))));
			}
		}
		else if(tile != TileId::Air && !(tile == TileId::Water))
			DrawTile(mat, surface, x, y, tile);

	}

	DrawPlayer(mat, surface);

	for(const World::Platform& platform : world_.GetPlatforms())
		DrawPlatform(mat, surface, platform);

	for(const World::Monster& monster : world_.GetMonsters())
		DrawMonster(mat, surface, monster);

	for(const World::PowerUp& power_up : world_.GetPowerUps())
		DrawPowerUp(mat, surface, power_up);

	for(const World::Projectile& projectile : world_.GetProjectiles())
		DrawProjectile(mat, surface, projectile);

	for(const World::Explosion& explosion : world_.GetExplosions())
		DrawExplosion(mat, surface, explosion);

	if(player.GetHealth() > 0) // Do not draw aim direction if player is dead.
	{
		const fixed16vec2_t player_pos= VecMatMul(world_.GetPlayer().GetPos(), mat);
		const fixed16vec2_t aim_vec= world_.GetPlayer().GetAimNormal();
		if(aim_vec[0] != 0)
		{
			const int32_t c_aim_length= 128;
			const fixed16_t aim_vec_x_abs= Fixed16Abs(aim_vec[0]);
			for(
				int32_t i= 0, i_end= Fixed16RoundToInt(c_aim_length * aim_vec_x_abs), x= Fixed16RoundToInt(player_pos[0]), x_step= aim_vec[0] > 0 ? 1 : -1, y= player_pos[1], y_step= Fixed16Div(aim_vec[1], aim_vec_x_abs);
				i < i_end && x >= 0 && x < surface.w;
				++i, x+= x_step, y+= y_step)
			{
				const int32_t y_int= Fixed16RoundToInt(y);
				if(y_int >= 0 && y_int < surface.h)
					reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y_int)[x]= 0xFFFFFFFF;
			}
		}
	}

	// Draw transparent water after all other geometry.
	for(uint32_t y= 0; y < tiles_map.GetSizeY(); ++y)
	for(uint32_t x= 0; x < tiles_map.GetSizeX(); ++x)
	{
		const TileId tile= tiles_map.GetTile(x, y);

		if(tile == TileId::Water)
			DrawTile(mat, surface, x, y, tile);
	}
}

void WorldView::DrawTile(const TransformMatrix& view_mat, const SDL_Surface& surface, const uint32_t tile_x, const uint32_t tile_y, const TileId tile)
{
	TransformMatrix tile_mat;
	tile_mat.scale[0]= tile_mat.scale[1] = g_fixed16_one;
	tile_mat.shift[0]= IntToFixed16(int32_t(tile_x));
	tile_mat.shift[1]= IntToFixed16(int32_t(tile_y));

	const TransformMatrix result_mat= MatrixMul(tile_mat, view_mat);

	const fixed16vec2_t tile_min{0, 0}, tile_max{g_fixed16_one, g_fixed16_one};
	const fixed16vec2_t tile_min_transformed= VecMatMul(tile_min, result_mat), tile_max_transformed= VecMatMul(tile_max, result_mat);

	color_t color= ColorRGB(0, 0, 0);
	bool use_blending= false;
	switch(tile)
	{
	case TileId::BasicWall:
		color= ColorRGB(120, 64, 16);
		break;
	case TileId::Water:
		color= ColorRGB(64, 64, 220);
		use_blending= true;
		break;
	case TileId::Lava:
		color= ColorRGB(220, 130, 32);
		break;
	case TileId::KeyDoor0:
	case TileId::KeyDoor1:
	case TileId::KeyDoor2:
	case TileId::KeyDoor3:
		color= ColorRGB(128, 128, 128);
		break;
	default:
		break;
	};

	if(use_blending)
		FillRectangleWithBlending(
			surface,
			Fixed16RoundToInt(tile_min_transformed[0]), Fixed16RoundToInt(tile_min_transformed[1]),
			Fixed16RoundToInt(tile_max_transformed[0]), Fixed16RoundToInt(tile_max_transformed[1]),
			color);
	else
		FillRectangle(
			surface,
			Fixed16RoundToInt(tile_min_transformed[0]), Fixed16RoundToInt(tile_min_transformed[1]),
			Fixed16RoundToInt(tile_max_transformed[0]), Fixed16RoundToInt(tile_max_transformed[1]),
			color);
}

void WorldView::DrawPlayer(const TransformMatrix& view_mat, const SDL_Surface& surface)
{
	const Player& player= world_.GetPlayer();

	TransformMatrix player_mat;
	player_mat.scale[0]= player.GetAimNormal()[0] >= 0 ? (+g_fixed16_one) : (-g_fixed16_one);
	player_mat.scale[1]= g_fixed16_one;
	player_mat.shift[0]= player.GetPos()[0];
	player_mat.shift[1]= player.GetPos()[1];

	if(player.GetHealth() <= 0)
	{
		player_mat.scale[1]/= 4;
		player_mat.shift[1]+= g_fixed16_one * 3 / 4;
	}

	const TransformMatrix result_mat= MatrixMul(player_mat, view_mat);

	DrawModel(result_mat, surface, Models::player);
}

void WorldView::DrawPlatform(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::Platform& platform)
{
	TransformMatrix platform_mat;
	platform_mat.scale[0]= platform_mat.scale[1] = g_fixed16_one;
	platform_mat.shift= platform.pos;

	if(platform.vel[0] <0)
		platform_mat.scale[0]*= -1;

	const TransformMatrix result_mat= MatrixMul(platform_mat, view_mat);

	DrawModel(result_mat, surface, Models::platform);
}

void WorldView::DrawMonster(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::Monster& monster)
{
	if(monster.health <= 0)
		return; // TODO - draw dead body?

	TransformMatrix monster_mat;
	monster_mat.scale[0]= monster_mat.scale[1] = g_fixed16_one;
	monster_mat.shift= monster.pos;

	if(monster.move_dir <0)
		monster_mat.scale[0]*= -1;

	const TransformMatrix result_mat= MatrixMul(monster_mat, view_mat);

	DrawModel(result_mat, surface, GetModelForMonster(monster.id));
}

void WorldView::DrawPowerUp(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::PowerUp& power_up)
{
	if(power_up.picked_up)
		return;

	TransformMatrix power_up_mat;
	power_up_mat.scale[0]= power_up_mat.scale[1] = g_fixed16_one;
	power_up_mat.shift= power_up.pos;

	const TransformMatrix result_mat= MatrixMul(power_up_mat, view_mat);

	DrawModel(result_mat, surface, GetModelForPowerup(power_up.id));
}

void WorldView::DrawProjectile(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::Projectile& projectile)
{
	const ProjectileModel model= GetProjectileModel(projectile.kind);
	const fixed16vec2_t center_projected= VecMatMul(projectile.pos, view_mat);

	const fixed16_t x_plus_edge_projected= VecMatMul({projectile.pos[0] + model.radius, projectile.pos[1]}, view_mat)[0];
	const fixed16_t radius_projected= x_plus_edge_projected - center_projected[0];

	FillCircle(surface, center_projected, radius_projected, model.color);
}

void WorldView::DrawExplosion(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::Explosion& explosion)
{
	const float relative_age= 1.5f + float(explosion.age) / float(World::Explosion::c_max_age);
	const fixed16_t radius= fixed16_t(relative_age * relative_age * float(g_fixed16_one) / 48.0f);
	const float ratio_shift= float(explosion.age) / float(World::Explosion::c_max_age) - 0.5f;

	const fixed16vec2_t center_projected= VecMatMul(explosion.pos, view_mat);

	const fixed16_t x_plus_edge_projected= VecMatMul({explosion.pos[0] + radius, explosion.pos[1]}, view_mat)[0];
	const fixed16_t radius_projected= x_plus_edge_projected - center_projected[0];

	DrawParticlesCircle(surface, center_projected, radius_projected, ratio_shift, ColorRGB(255, 255, 255));
}

void WorldView::DrawModel(const TransformMatrix& mat, const SDL_Surface& surface, const Model& model)
{
	for(const ModelTrapezoid& trapeziod : model)
	{
		fixed16vec2_t start_0= VecMatMul({trapeziod.sides[0].x[0], trapeziod.sides[0].y}, mat);
		fixed16vec2_t start_1= VecMatMul({trapeziod.sides[0].x[1], trapeziod.sides[0].y}, mat);
		fixed16vec2_t end_0= VecMatMul({trapeziod.sides[1].x[0], trapeziod.sides[1].y}, mat);
		fixed16vec2_t end_1= VecMatMul({trapeziod.sides[1].x[1], trapeziod.sides[1].y}, mat);
		if(mat.scale[0] < 0)
		{
			std::swap(start_0[0], start_1[0]);
			std::swap(end_0[0], end_1[0]);
		}
		if(mat.scale[1] < 0)
			std::swap(start_0[1], end_0[1]);

		FillTrapezoid(
			surface,
			start_0[1], end_0[1],
			start_0[0], start_1[0],
			end_0[0], end_1[0], trapeziod.color);
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

void WorldView::FillRectangleWithBlending(const SDL_Surface& surface, const int32_t min_x, const int32_t min_y, const int32_t max_x, const int32_t max_y, color_t color)
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

	const color_t color_prepared= color >> 1;
	for(int32_t y= min_y_clamped; y < max_y_clamped; ++y)
	for(int32_t x= min_x_clamped; x < max_x_clamped; ++x)
	{
		auto& dst= reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y)[x];
		dst= ((dst & 0xFEFEFEFEu) >> 1) + color_prepared;
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

void WorldView::DrawParticlesCircle(const SDL_Surface& surface, const fixed16vec2_t& center, const fixed16_t radius, const float ratio_shift, const color_t color)
{
	// TODO - get rid of floats.
	const float c_pi= 3.1415926535f;
	const float c_phi= (std::sqrt(5.0f) - 1.0f) / 2.0f;
	const float c_golden_angle= 2.0f * c_pi * (1.0f - c_phi);
	const float radius_f= float(radius) / float(g_fixed16_one);
	for(size_t i= 0; i < 256; ++i)
	{
		const float f_i= float(i);
		const float current_radius= radius_f * std::sqrt(f_i);
		const float angle= f_i * (c_golden_angle + ratio_shift * 0.01f);
		float dx= current_radius * std::cos(angle);
		float dy= current_radius * std::sin(angle);

		const fixed16_t center_x= center[0] + fixed16_t(dx * float(g_fixed16_one));
		const fixed16_t center_y= center[1] + fixed16_t(dy * float(g_fixed16_one));

		const int32_t x= Fixed16RoundToInt(center_x);
		const int32_t y= Fixed16RoundToInt(center_y);
		if(x >= 0 && x < surface.w && y >= 0 && y < surface.h)
			reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y)[x]= color;

		//FillCircle(surface, {center_x, center_y}, g_fixed16_one * 5 / 2, color);
	}
}

} // namespace Armed
