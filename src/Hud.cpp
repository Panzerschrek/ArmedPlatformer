#include "Hud.hpp"

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
	const int32_t c_health_circle_offset_x= 32;
	const int32_t c_health_circle_offset_y= 32;
	const int32_t c_health_circle_inner_radius= 10;
	const int32_t c_health_circle_outer_radius= 24;
	const int32_t c_health_circle_outer_extra_radius= 25;
	const int32_t c_boss_health_circle_offset_y= 48;
	const color_t c_health_circle_color= ColorRGB(220, 32, 32);
	const color_t c_health_circle_extra_color= ColorRGB(180, 180, 180);
	const color_t c_boss_health_circle_color= ColorRGB(119, 163, 89);

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

	const Player& player= world_.GetPlayer();

	const int32_t health= std::max(0, std::min(player.GetHealth(), Player::c_max_health));

	DrawRoundIndicator(
		surface,
		c_health_circle_offset_x,
		surface_height - c_health_circle_offset_y,
		c_health_circle_inner_radius, c_health_circle_outer_radius,
		c_health_circle_color,
		IntToFixed16(health) / Player::c_max_health);

	DrawRoundIndicator(
		surface,
		c_health_circle_offset_x,
		surface_height - c_health_circle_offset_y,
		c_health_circle_outer_radius, c_health_circle_outer_extra_radius,
		c_health_circle_extra_color,
		g_fixed16_one);

	const int32_t c_max_ammo= 48;
	const color_t c_ammo_colors[]{ColorRGB(200, 200, 200), ColorRGB(220, 120, 64), ColorRGB(128, 32, 32)};
	for(int32_t i= 0; i < 3; ++i)
	{
		const int32_t c_ammo_bar_width= 4;
		const int32_t c_ammo_bar_height= 8;
		const int32_t c_ammo_bar_margin= 2;
		const int32_t c_ammo_bar_row_length= 16;

		const int32_t ammo_bar_x= surface_width - ((c_ammo_bar_width + c_ammo_bar_margin) * c_ammo_bar_row_length + c_ammo_bar_margin * 2) * (i + 1);
		const int32_t ammo_bar_y= surface_height - c_ammo_bar_margin - (c_ammo_bar_height + c_ammo_bar_margin) * (c_max_ammo / c_ammo_bar_row_length);

		DrawAmmoIndicator(
			surface,
			ammo_bar_x,
			ammo_bar_y,
			c_ammo_bar_width, c_ammo_bar_height,
			c_ammo_bar_margin, c_ammo_bar_margin,
			c_ammo_bar_row_length,
			player.GetAmmo(size_t(i)),
			c_ammo_colors[i]);

		if(size_t(i) == player.GetCurrentWeapon())
		{
			const int32_t start_x= ammo_bar_x, end_x= start_x + (c_ammo_bar_width + c_ammo_bar_margin) * c_ammo_bar_row_length - c_ammo_bar_margin;
			const int32_t start_y= ammo_bar_y - c_ammo_bar_height - c_ammo_bar_margin * 2, end_y= start_y + c_ammo_bar_height;

			for(int32_t y= start_y; y < end_y; ++y)
			{
				auto dst= reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y);
				for(int32_t x= start_x; x < end_x; ++x)
					dst[x]= c_ammo_colors[i];
			}
		}
	}

	for(const World::Monster& monster : world_.GetMonsters())
	{
		if(monster.id == MonsterId::Boss)
		{
			DrawRoundIndicator(
				surface,
				surface.w / 2,
				c_boss_health_circle_offset_y,
				c_health_circle_inner_radius, c_health_circle_outer_radius,
				c_boss_health_circle_color,
				IntToFixed16(monster.health) / World::c_boss_health);

			DrawRoundIndicator(
				surface,
				surface.w / 2,
				c_boss_health_circle_offset_y,
				c_health_circle_outer_radius, c_health_circle_outer_extra_radius,
				c_health_circle_extra_color,
				g_fixed16_one);
		}
	}
}

void Hud::DrawRoundIndicator(
	const SDL_Surface& surface,
	const int32_t center_x,
	const int32_t center_y,
	const int32_t inner_radius, const int32_t outer_radius,
	color_t color,
	const fixed16_t value)
{
	const int32_t square_inner_radius= inner_radius * inner_radius;
	const int32_t square_outer_radius= outer_radius * outer_radius;

	const float value_angle= float(value) / float(g_fixed16_one) * (2.0f * 3.1415926535f);
	const fixed16_t value_cos= fixed16_t(std::cos(value_angle) * float(g_fixed16_one));
	const fixed16_t square_cos= Fixed16Mul(value_cos, value_cos);

	const int32_t y_end= value >= (g_fixed16_one / 4) ? (center_y + outer_radius) : center_y;
	for(int32_t y= center_y - outer_radius; y <= y_end; ++y)
	{
		const int32_t dy = y - center_y;
		const int32_t dy2= dy * dy;
		auto dst= reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y);
		const int32_t x_end= value >= (g_fixed16_one / 2) ? (center_x + outer_radius) : center_x;
		for(int32_t x= center_x - outer_radius; x <= x_end; ++x)
		{
			const int32_t dx= x - center_x;
			const int32_t dx2= dx * dx;
			const int32_t square_dist= dx2 + dy2;
			if(square_dist >= square_inner_radius && square_dist < square_outer_radius)
			{
				const fixed16_t angle_cos2= IntToFixed16(dy2) / square_dist;
				bool draw= dx < 0;
				if(value >= g_fixed16_one / 2)
				{
					if(value >= g_fixed16_one * 3 / 4)
						draw|= dy > 0 || angle_cos2 <= square_cos;
					else
						draw|= dy > 0 && angle_cos2 > square_cos;
				}
				else
				{
					if(value >= g_fixed16_one / 4)
						draw&= dy <= 0 || angle_cos2 <= square_cos;
					else
						draw&= dy <= 0 && angle_cos2 > square_cos;
				}

				if(draw)
					dst[x]= color;
			}
		}
	}
}

void Hud::DrawAmmoIndicator(
	const SDL_Surface& surface,
	const int32_t start_x, const int32_t start_y,
	const int32_t bar_size_x, const int32_t bar_size_y,
	const int32_t bar_margin_x, const int32_t bar_margin_y,
	const uint32_t row_length,
	const uint32_t count,
	const color_t color)
{
	for(uint32_t i= 0; i < count; ++i)
	{
		const uint32_t row= i / row_length;
		const uint32_t column= i % row_length;

		const int32_t current_x= start_x + (bar_size_x + bar_margin_x) * int32_t(column);
		const int32_t current_y= start_y + (bar_size_y + bar_margin_y) * int32_t(row);

		for(int32_t y= current_y; y < current_y + bar_size_y; ++y)
		{
			auto dst= reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y);
			for(int32_t x= current_x; x < current_x + bar_size_x; ++x)
				dst[x]= color;
		}
	}
}

} // namespace Armed
