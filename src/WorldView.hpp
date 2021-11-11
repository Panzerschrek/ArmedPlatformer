#pragma once
#include "Camera.hpp"
#include "SystemWindow.hpp"
#include "World.hpp"
#include "Mat.hpp"
#include "Model.hpp"

namespace Armed
{

class WorldView
{
public:
	WorldView(const World& world, const Camera& camera, SystemWindow& system_window);

	void Draw();

private:
	void DrawTile(const TransformMatrix& view_mat, const SDL_Surface& surface, uint32_t tile_x, uint32_t tile_y, TileId tile);
	void DrawPlayer(const TransformMatrix& view_mat, const SDL_Surface& surface);
	void DrawPlatform(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::Platform& platform);
	void DrawMonster(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::Monster& monster);
	void DrawPowerUp(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::PowerUp& power_up);
	void DrawProjectile(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::Projectile& projectile);
	void DrawExplosion(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::Explosion& explosion);
	void DrawModel(const TransformMatrix& mat, const SDL_Surface& surface, const Model& model);
	void FillRectangle(const SDL_Surface& surface, int32_t min_x, int32_t min_y, int32_t max_x, int32_t max_y, color_t color);
	void FillRectangleWithBlending(const SDL_Surface& surface, int32_t min_x, int32_t min_y, int32_t max_x, int32_t max_y, color_t color);

	void FillTrapezoid(
		const SDL_Surface& surface,
		fixed16_t y_start, fixed16_t y_end, // y_end >= y_start
		fixed16_t x_start_0, fixed16_t x_start_1,
		fixed16_t x_end_0, fixed16_t x_end_1,
		color_t color);

	// Max radius ~ 127 pixels.
	void FillCircle(const SDL_Surface& surface, const fixed16vec2_t& center, fixed16_t radius, color_t color);

	void DrawParticlesCircle(const SDL_Surface& surface, const fixed16vec2_t& center, fixed16_t radius, color_t color);

private:
	const World& world_;
	const Camera& camera_;
	SystemWindow& system_window_;
};

} // namespace Armed
