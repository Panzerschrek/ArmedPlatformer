#pragma once
#include "Mat.hpp"
#include "SystemWindow.hpp"
#include "World.hpp"

namespace Armed
{

class Camera
{
public:
	Camera(const World& world, SystemWindow& system_window);

	// Single update frame.
	void Update(const InputState& input_state);

	TransformMatrix CalculateMatrix() const;
	const fixed16vec2_t GetAimVec() const { return aim_vec_; }

private:
	const World& world_;
	SystemWindow& system_window_;

	fixed16vec2_t pos_{};
	fixed16vec2_t aim_vec_{};
	fixed16_t scale_= 0;
};

} // namespace Armed
