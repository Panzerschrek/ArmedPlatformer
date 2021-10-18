#pragma once
#include "SystemWindow.hpp"
#include "World.hpp"

namespace Armed
{

// new_x = x * scale + shift[0]
// new_y = y * scale + shift[1]
struct TransformMatrix
{
	fixed16_t shift[2];
	fixed16_t scale;
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
	TransformMatrix CalculateViewTransformMatrix();
	void DrawTile(const TransformMatrix& view_mat, const SDL_Surface& surface, uint32_t tile_x, uint32_t tile_y, TileId tile);

private:
	const World& world_;
	SystemWindow& system_window_;
};

} // namespace Armed
