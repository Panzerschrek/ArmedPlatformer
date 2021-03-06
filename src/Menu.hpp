#pragma once
#include "Color.hpp"
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
		std::function<void(size_t slot)> save_game;
		std::function<void(size_t slot)> load_game;
		std::function<void()> quit;
		std::function<bool()> save_available;
		std::function<bool(size_t slot)> has_save;
	};

public:
	explicit Menu(SystemWindow& system_window, Callbacks callbacks);

	void ProcessInput(const SystemEvents& system_events);

	void Draw();

	bool IsActive() const { return cursor_pos_ != CursorPos::None; }

private:
	constexpr static size_t c_num_save_slots= 5;
	enum class CursorPos
	{
		None, // Menu is not active.
		NewGame,
		SaveGame,
		SaveSlot0,
		SaveSlotLast= SaveSlot0 + c_num_save_slots,
		LoadGame,
		LoadSlot0,
		LoadSlotLast= LoadSlot0 + c_num_save_slots,
		Quit,
	};

private:
	void ProcessKeyPress(SystemEventTypes::KeyCode code);

	// TODO - remove copy-paste.
	void FillRectangle(const SDL_Surface& surface, int32_t min_x, int32_t min_y, int32_t max_x, int32_t max_y, color_t color);

private:
	SystemWindow& system_window_;
	Callbacks callbacks_;
	CursorPos cursor_pos_= CursorPos::NewGame;
	bool has_saves_[c_num_save_slots]{};
};

} // namespace Armed
