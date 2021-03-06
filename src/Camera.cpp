#include "Camera.hpp"

namespace Armed
{

namespace
{

const fixed16_t g_default_scale= IntToFixed16(32);
const fixed16_t g_max_scale= IntToFixed16(64);
const fixed16_t g_min_scale= IntToFixed16(16);

} // namespace

Camera::Camera(const World& world, SystemWindow& system_window)
	: world_(world), system_window_(system_window), pos_(world_.GetPlayer().GetPos()), scale_(g_default_scale)
{
}

void Camera::Update(const InputState& input_state)
{
	const fixed16vec2_t& player_pos= world_.GetPlayer().GetPos();
	{
		// TODO - move make target position towards movement direction?
		const fixed16vec2_t target_pos= player_pos;

		// Smooth camera movement.
		const fixed16_t f= g_fixed16_one / 24;
		for(size_t i= 0; i < 2; ++i)
			pos_[i]= Fixed16VecDot({pos_[i], target_pos[i]}, {g_fixed16_one - f, f});
	}

	const fixed16vec2_t player_pos_transformed= VecMatMul(player_pos, CalculateMatrix());

	aim_vec_[0]= IntToFixed16(input_state.cursor_pos[0]) + g_fixed16_one / 2 - player_pos_transformed[0];

	aim_vec_[1]= IntToFixed16(input_state.cursor_pos[1]) + g_fixed16_one / 2 - player_pos_transformed[1];

	const fixed16_t c_scale_step= g_fixed16_one / 4;
	if(input_state.keyboard[size_t(SystemEventTypes::KeyCode::Equals)])
		scale_+= c_scale_step;
	if(input_state.keyboard[size_t(SystemEventTypes::KeyCode::Minus)])
		scale_-= c_scale_step;

	scale_= std::max(g_min_scale, std::min(scale_, g_max_scale));
}

TransformMatrix Camera::CalculateMatrix() const
{
	const SDL_Surface& surface= system_window_.GetSurface();

	TransformMatrix cam_shift{}, scale{}, screen_shift{};

	cam_shift.scale[0]= cam_shift.scale[1] = g_fixed16_one;
	cam_shift.shift[0]= -pos_[0];
	cam_shift.shift[1]= -pos_[1];

	scale.scale[0]= scale.scale[1]= scale_;
	scale.shift[0]= scale.shift[1]= 0;

	screen_shift.scale[0]= screen_shift.scale[1]= g_fixed16_one;
	screen_shift.shift[0]= IntToFixed16(surface.w) / 2;
	screen_shift.shift[1]= IntToFixed16(surface.h) / 2;

	return MatrixMul(MatrixMul(cam_shift, scale), screen_shift);
}

} // namespace Armed
