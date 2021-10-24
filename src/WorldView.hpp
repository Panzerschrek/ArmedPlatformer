#pragma once
#include "SystemWindow.hpp"
#include "World.hpp"
#include "Model.hpp"

namespace Armed
{

// new_x = x * scale[0] + shift[0]
// new_y = y * scale[1] + shift[1]
struct TransformMatrix
{
	fixed16_t shift[2];
	fixed16_t scale[2];
};

TransformMatrix MatrixMul(const TransformMatrix& l, const TransformMatrix& r);
fixed16_t TransformX(const TransformMatrix& mat, fixed16_t x);
fixed16_t TransformY(const TransformMatrix& mat, fixed16_t y);


class WorldView
{
public:
	WorldView(const World& world, SystemWindow& system_window);

	void Draw();

private:
	TransformMatrix CalculateViewTransformMatrix(const SDL_Surface& surface);
	void DrawTile(const TransformMatrix& view_mat, const SDL_Surface& surface, uint32_t tile_x, uint32_t tile_y, TileId tile);
	void DrawPlayer(const TransformMatrix& view_mat, const SDL_Surface& surface);
	void DrawMonster(const TransformMatrix& view_mat, const SDL_Surface& surface, const World::Monster& monster);
	void DrawModel(const TransformMatrix& mat, const SDL_Surface& surface, const Model& model);
	void FillRectangle(const SDL_Surface& surface, int32_t min_x, int32_t min_y, int32_t max_x, int32_t max_y, pixel_t color);

	void FillTrapezoid(
		const SDL_Surface& surface,
		fixed16_t y_start, fixed16_t y_end, // y_end >= y_start
		fixed16_t x_start_0, fixed16_t x_start_1,
		fixed16_t x_end_0, fixed16_t x_end_1,
		pixel_t color);

private:
	const World& world_;
	SystemWindow& system_window_;
};

} // namespace Armed
