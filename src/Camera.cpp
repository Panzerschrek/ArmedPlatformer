#include "Camera.hpp"

namespace Armed
{

Camera::Camera(const World& world, SystemWindow& system_window)
	: world_(world), system_window_(system_window), pos_(world_.GetPlayer().GetPos())
{
}

void Camera::Update()
{
	// TODO - move make target position towards movement direction?
	const fixed16vec2_t target_pos= world_.GetPlayer().GetPos();

	// Smooth camera movement.
	const fixed16_t f= g_fixed16_one / 24;
	for(size_t i= 0; i < 2; ++i)
		pos_[i]= Fixed16VecDot({pos_[i], target_pos[i]}, {g_fixed16_one - f, f});
}

TransformMatrix Camera::CalculateMatrix() const
{
	const SDL_Surface& surface= system_window_.GetSurface();

	TransformMatrix cam_shift{}, scale{}, screen_shift{};

	cam_shift.scale[0]= cam_shift.scale[1] = g_fixed16_one;
	cam_shift.shift[0]= -pos_[0];
	cam_shift.shift[1]= -pos_[1];

	scale.scale[0]= scale.scale[1]= IntToFixed16(24);
	scale.shift[0]= scale.shift[1]= 0;

	screen_shift.scale[0]= screen_shift.scale[1]= g_fixed16_one;
	screen_shift.shift[0]= IntToFixed16(surface.w) / 2;
	screen_shift.shift[1]= IntToFixed16(surface.h) / 2;

	return MatrixMul(MatrixMul(cam_shift, scale), screen_shift);
}

} // namespace Armed
