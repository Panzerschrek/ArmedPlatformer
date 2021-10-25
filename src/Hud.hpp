#pragma once
#include "Color.hpp"
#include "SystemWindow.hpp"
#include "World.hpp"

namespace Armed
{

class Hud
{
public:
	Hud(const World& world, SystemWindow& system_window);

	void Draw();

private:
	void DrawRoundIndicator(
		const SDL_Surface& surface,
		int32_t center_x, int32_t center_y,
		int32_t inner_radius, int32_t outer_radius,
		color_t color,
		fixed16_t value);

private:
	const World& world_;
	SystemWindow& system_window_;
};

} // namespace Armed
