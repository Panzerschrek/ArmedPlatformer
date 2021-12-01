#include "Menu.hpp"
#include "Assert.hpp"

namespace Armed
{

Menu::Menu(SystemWindow& system_window, Callbacks callbacks)
	: system_window_(system_window), callbacks_(std::move(callbacks))
{
}

void Menu::ProcessInput(const SystemEvents& system_events)
{
	for(const SystemEvent& event : system_events)
	{
		if(const auto key_event= std::get_if<SystemEventTypes::KeyEvent>(&event))
		{
			if(key_event->pressed)
				ProcessKeyPress(key_event->key_code);
		}
	}
}

void Menu::Draw()
{
	if(cursor_pos_ == CursorPos::None)
		return;

	const SDL_Surface& surface= system_window_.GetSurface();

	// Make background darker.
	// TODO - remove copy-paste.
	for(int32_t y= 0; y < surface.h; ++y)
	{
		auto dst= reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y);
		for(int32_t x= 0; x < surface.w; ++x)
			dst[x]= (dst[x] & 0xFEFEFEFE) >> 1;
	}

	const int32_t base_width= 640;

	const int32_t c_menu_bar_width= 240;
	const int32_t c_menu_bar_height= 48;
	const int32_t c_menu_bar_margin= 16;

	const int32_t c_cursor_width= 32;
	const int32_t c_cursor_height= 32;
	const int32_t c_cursor_margin= 8;

	const int32_t c_save_bar_width= 144;
	const int32_t c_save_bar_height= 32;
	const int32_t c_save_bar_margin= 12;

	const int32_t c_save_cursor_width= 24;
	const int32_t c_save_cursor_height= 24;
	const int32_t c_save_cursor_margin= 4;

	const int32_t c_menu_start_x= 64;
	const int32_t c_menu_start_y= 48;

	const color_t c_color= ColorRGB(64, 255, 128);
	const color_t c_cursor_color= ColorRGB(48, 220, 80);
	const color_t c_unactive_color= ColorRGB(128, 128, 128);

	const int32_t cur_menu_start_x= c_menu_start_x + (surface.w - base_width) / 2 + c_cursor_margin + c_cursor_width;
	{
		int32_t cur_x= cur_menu_start_x;
		int32_t cur_y= c_menu_start_y;

		const CursorPos base_cursors[4]{CursorPos::NewGame, CursorPos::SaveGame, CursorPos::LoadGame, CursorPos::Quit};
		for(CursorPos menu_bar : base_cursors)
		{
			const auto color= (menu_bar == CursorPos::SaveGame && !callbacks_.save_available()) ? c_unactive_color : c_color;
			FillRectangle(surface, cur_x, cur_y, cur_x + c_menu_bar_width, cur_y + c_menu_bar_height, color);

			if(cursor_pos_ == menu_bar)
			{
				const int32_t cursor_start_y= cur_y + (c_menu_bar_height - c_cursor_height ) / 2;
				FillRectangle(surface, cur_x - c_cursor_margin - c_cursor_width, cursor_start_y, cur_x - c_cursor_margin, cursor_start_y + c_cursor_height, c_cursor_color);
			}

			cur_y+= c_menu_bar_height + c_menu_bar_margin;
		}
	}

	size_t current_save_slot= c_num_save_slots;
	if(cursor_pos_ >= CursorPos::SaveSlot0 && cursor_pos_ <= CursorPos::SaveSlotLast)
		current_save_slot= size_t(cursor_pos_) - size_t(CursorPos::SaveSlot0);
	if(cursor_pos_ >= CursorPos::LoadSlot0 && cursor_pos_ <= CursorPos::LoadSlotLast)
		current_save_slot= size_t(cursor_pos_) - size_t(CursorPos::LoadSlot0);

	if(current_save_slot < c_num_save_slots)
	{
		int32_t cur_x= cur_menu_start_x + c_menu_bar_width + c_save_bar_margin + c_save_cursor_width + c_save_cursor_margin;
		int32_t cur_y= c_menu_start_y;

		for(size_t i= 0; i < c_num_save_slots; ++i)
		{
			const auto color= (cursor_pos_ >= CursorPos::LoadSlot0 && cursor_pos_ <= CursorPos::LoadSlotLast && !has_saves_[i]) ? c_unactive_color : c_color;
			FillRectangle(surface, cur_x, cur_y, cur_x + c_save_bar_width, cur_y + c_save_bar_height, color);
			if(current_save_slot == i)
				FillRectangle(surface, cur_x - c_save_cursor_margin - c_save_cursor_width, cur_y + c_save_cursor_margin, cur_x - c_save_cursor_margin, cur_y + c_save_cursor_margin + c_save_cursor_height, c_color);

			cur_y+= c_save_bar_height + c_save_bar_margin;
		}
	}
}

void Menu::ProcessKeyPress(const SystemEventTypes::KeyCode code)
{
	using KeyCode= SystemEventTypes::KeyCode;
	if(code == KeyCode::Escape)
	{
		if(cursor_pos_ == CursorPos::None)
			cursor_pos_= CursorPos::SaveGame;
		else if(cursor_pos_ == CursorPos::NewGame ||
				cursor_pos_ == CursorPos::SaveGame ||
				cursor_pos_ == CursorPos::LoadGame ||
				cursor_pos_ == CursorPos::Quit)
			cursor_pos_= CursorPos::None;
		else if(cursor_pos_ >= CursorPos::SaveSlot0 && cursor_pos_ <= CursorPos::SaveSlotLast)
			cursor_pos_= CursorPos::SaveGame;
		else if(cursor_pos_ >= CursorPos::LoadSlot0 && cursor_pos_ <= CursorPos::LoadSlotLast)
			cursor_pos_= CursorPos::LoadGame;
	}

	if(cursor_pos_ == CursorPos::None)
		return;

	if(code == KeyCode::Up)
	{
		if(cursor_pos_ == CursorPos::NewGame)
			cursor_pos_= CursorPos::Quit;
		else if(cursor_pos_ == CursorPos::SaveGame)
			cursor_pos_= CursorPos::NewGame;
		else if(cursor_pos_ == CursorPos::LoadGame)
			cursor_pos_= CursorPos::SaveGame;
		else if(cursor_pos_ == CursorPos::Quit)
			cursor_pos_= CursorPos::LoadGame;
		else if(cursor_pos_ >= CursorPos::SaveSlot0 && cursor_pos_ <= CursorPos::SaveSlotLast)
			cursor_pos_= CursorPos(size_t(CursorPos::SaveSlot0) + (size_t(cursor_pos_) - size_t(CursorPos::SaveSlot0) + c_num_save_slots - 1) % c_num_save_slots);
		else if(cursor_pos_ >= CursorPos::LoadSlot0 && cursor_pos_ <= CursorPos::LoadSlotLast)
			cursor_pos_= CursorPos(size_t(CursorPos::LoadSlot0) + (size_t(cursor_pos_) - size_t(CursorPos::LoadSlot0) + c_num_save_slots - 1) % c_num_save_slots);
	}
	else if(code == KeyCode::Down)
	{
		if(cursor_pos_ == CursorPos::NewGame)
			cursor_pos_= CursorPos::SaveGame;
		else if(cursor_pos_ == CursorPos::SaveGame)
			cursor_pos_= CursorPos::LoadGame;
		else if(cursor_pos_ == CursorPos::LoadGame)
			cursor_pos_= CursorPos::Quit;
		else if(cursor_pos_ == CursorPos::Quit)
			cursor_pos_= CursorPos::NewGame;
		else if(cursor_pos_ >= CursorPos::SaveSlot0 && cursor_pos_ <= CursorPos::SaveSlotLast)
			cursor_pos_= CursorPos(size_t(CursorPos::SaveSlot0) + (size_t(cursor_pos_) - size_t(CursorPos::SaveSlot0) + 1) % c_num_save_slots);
		else if(cursor_pos_ >= CursorPos::LoadSlot0 && cursor_pos_ <= CursorPos::LoadSlotLast)
			cursor_pos_= CursorPos(size_t(CursorPos::LoadSlot0) + (size_t(cursor_pos_) - size_t(CursorPos::LoadSlot0) + 1) % c_num_save_slots);
	}
	else if(code == KeyCode::Enter)
	{
		if(cursor_pos_ == CursorPos::NewGame)
		{
			callbacks_.new_game();
			cursor_pos_= CursorPos::None;
		}
		else if(cursor_pos_ == CursorPos::SaveGame)
		{
			if(callbacks_.save_available())
				cursor_pos_= CursorPos::SaveSlot0;
		}
		else if(cursor_pos_ == CursorPos::LoadGame)
		{
			cursor_pos_= CursorPos::LoadSlot0;
			for(size_t i= 0; i < c_num_save_slots; ++i)
				has_saves_[i]= callbacks_.has_save(i);
		}
		else if(cursor_pos_ == CursorPos::Quit)
			callbacks_.quit();
		else if(cursor_pos_ >= CursorPos::SaveSlot0 && cursor_pos_ <= CursorPos::SaveSlotLast)
		{
			if(callbacks_.save_available())
			{
				callbacks_.save_game(size_t(cursor_pos_) - size_t(CursorPos::SaveSlot0));
				cursor_pos_= CursorPos::None;
			}
		}
		else if(cursor_pos_ >= CursorPos::LoadSlot0 && cursor_pos_ <= CursorPos::LoadSlotLast)
		{
			const size_t slot= size_t(cursor_pos_) - size_t(CursorPos::LoadSlot0);
			if(has_saves_[slot])
			{
				callbacks_.load_game(slot);
				cursor_pos_= CursorPos::None;
			}
		}
	}
}

void Menu::FillRectangle(const SDL_Surface& surface, const int32_t min_x, const int32_t min_y, const int32_t max_x, const int32_t max_y, color_t color)
{
	if(min_x >= surface.w ||
		min_y >= surface.h ||
		max_x <= 0 ||
		max_y <= 0)
		return;

	const int32_t min_x_clamped= std::max(0, min_x);
	const int32_t min_y_clamped= std::max(0, min_y);
	const int32_t max_x_clamped= std::min(max_x, surface.w);
	const int32_t max_y_clamped= std::min(max_y, surface.h);

	for(int32_t y= min_y_clamped; y < max_y_clamped; ++y)
	for(int32_t x= min_x_clamped; x < max_x_clamped; ++x)
	{
		auto& dst= reinterpret_cast<color_t*>(static_cast<char*>(surface.pixels) + surface.pitch * y)[x];
		dst= color;
	}
}

} // namespace Armed
