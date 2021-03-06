#pragma once
#include "Hud.hpp"
#include "Menu.hpp"
#include "SoundProcessor.hpp"
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

	struct CurrentWolrdData
	{
		World world;
		Camera camera;
		WorldView world_view;
		Hud hud;

		CurrentWolrdData(
			const MapDescription& map_description,
			SystemWindow& system_window,
			SoundProcessor& sound_processor);
		CurrentWolrdData(World in_world, SystemWindow& system_window);
	};

private:
	TimePoint GetCurrentTime();
	void NewGame(size_t map_index= 0);
	void SaveGame(size_t slot);
	void LoadGame(size_t slot);
	void Quit();
	bool SaveAvailable();
	bool HasSave(size_t slot);

private:
	SystemWindow system_window_;
	SoundOut sound_out_;
	SoundProcessor sound_processor_;
	std::optional<CurrentWolrdData> current_world_data_;
	Menu menu_;

	const Clock::time_point init_time_;
	TimePoint prev_tick_time_;
	bool quit_requested_= false;
	size_t current_map_= 0;
};

} // namespace Armed
