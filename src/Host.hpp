#pragma once
#include "Hud.hpp"
#include "Menu.hpp"
#include "WorldView.hpp"
#include <chrono>

namespace Armed
{

class Host
{
public:
	Host();

	// Returns false on quit
	bool Loop();

private:
	using TimePoint = uint64_t;
	using ChronoDuration= std::chrono::milliseconds;
	static constexpr const uint64_t c_time_point_resolution= ChronoDuration::period::den; // time points in seconds

	using Clock= std::chrono::steady_clock;
	TimePoint GetCurrentTime();

private:
	void NewGame();
	void SaveGame(size_t slot);
	void LoadGame(size_t slot);
	void Quit();

private:
	SystemWindow system_window_;
	World world_;
	Camera camera_;
	WorldView world_view_;
	Hud hud_;
	Menu menu_;

	const Clock::time_point init_time_;
	TimePoint prev_tick_time_;
	bool quit_requested_= false;
};

} // namespace Armed
