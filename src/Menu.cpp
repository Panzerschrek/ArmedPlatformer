#include "Menu.hpp"
#include "Assert.hpp"

namespace Armed
{

Menu::Menu(SystemWindow& system_window, Callbacks callbacks)
	: system_window_(system_window), callbacks_(std::move(callbacks))
{
}

void Menu::Draw()
{
	const SDL_Surface& surface= system_window_.GetSurface();
	// TODO
	ARMED_UNUSED(surface);
}

} // namespace Armed
