#include "Assert.hpp"
#include "Host.hpp"
#include "Maps/Maps.hpp"
#include <thread>

namespace Armed
{

Host::Host()
	: system_window_()
	, world_(Maps::Test::description)
	, world_view_(world_, system_window_)
	, init_time_(Clock::now())
	, prev_tick_time_(GetCurrentTime())
{
}

bool Host::Loop()
{
	const auto tick_start_time= GetCurrentTime();

	const SystemEvents system_events= system_window_.ProcessEvents();
	for(const SystemEvent& system_event : system_events)
	{
		if(std::get_if<SystemEventTypes::QuitEvent>(&system_event) != nullptr)
			return true;
	}

	// Perform some ticks. Possible 0, 1 or many. But do not perform more than 5 ticks once.
	for (
		uint64_t
			physics_start_tick = prev_tick_time_ * World::c_update_frequency / c_time_point_resolution,
			physics_end_tick = tick_start_time * World::c_update_frequency / c_time_point_resolution,
			t= physics_start_tick,
			iterations= 0;
		t < physics_end_tick && iterations < 5;
		++t, ++iterations)
	{
		world_.Tick(system_window_.GetInputState());
	}

	system_window_.BeginFrame();
	world_view_.Draw();
	system_window_.EndFrame();

	const TimePoint tick_end_time= GetCurrentTime();
	const auto frame_dt= tick_end_time - tick_start_time;

	const uint64_t max_fps= 120;
	const auto min_frame_duration = c_time_point_resolution / max_fps;
	if(frame_dt <= min_frame_duration)
	{
		std::this_thread::sleep_for(ChronoDuration(min_frame_duration - frame_dt));
	}

	prev_tick_time_= tick_start_time;

	return false;
}

Host::TimePoint Host::GetCurrentTime()
{
	const Clock::time_point now= Clock::now();
	const auto dt= now - init_time_;

	return TimePoint(std::chrono::duration_cast<ChronoDuration>(dt).count());
}

} // namespace Armed
