#pragma once
#include "SystemEvent.hpp"
#include <SDL_video.h>

namespace Armed
{

class SystemWindow
{
public:
	SystemWindow();
	~SystemWindow();

	SystemEvents ProcessEvents();
	InputState GetInputState();

	const SDL_Surface& GetSurface() const;

	void BeginFrame();
	void EndFrame();

private:
	SDL_Window* window_= nullptr;
	SDL_Surface* surface_= nullptr;
};

} // namespace Armed
