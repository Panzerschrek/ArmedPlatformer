#pragma once
#include "SystemWindow.hpp"
#include <functional>

namespace Armed
{

class Menu
{
public:
	struct Callbacks
	{
		std::function<void()> new_game;
		std::function<void()> save_game;
		std::function<void()> load_game;
		std::function<void()> quit;
	};

public:
	explicit Menu(SystemWindow& system_window, Callbacks callbacks);

	void Draw();

private:
	SystemWindow& system_window_;
	Callbacks callbacks_;
};

} // namespace Armed
