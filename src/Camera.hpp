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
	void Update();

	TransformMatrix CalculateMatrix() const;

private:
	const World& world_;
	SystemWindow& system_window_;

	fixed16vec2_t pos_;
};

} // namespace Armed
