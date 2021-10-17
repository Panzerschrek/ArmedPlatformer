#include "Assert.hpp"
#include "Host.hpp"
#include <thread>

namespace Armed
{

Host::Host()
	:  system_window_()
	, init_time_(Clock::now())
	, prev_tick_time_(init_time_)
{
}

bool Host::Loop()
{
	const Clock::time_point tick_start_time= Clock::now();
	const auto dt= tick_start_time - prev_tick_time_;
	prev_tick_time_ = tick_start_time;

	const SystemEvents system_events= system_window_.ProcessEvents();
	for(const SystemEvent& system_event : system_events)
	{
		if(std::get_if<SystemEventTypes::QuitEvent>(&system_event) != nullptr)
			return true;
	}

	ARMED_UNUSED(dt); // TODO - use it

	system_window_.BeginFrame();
	system_window_.EndFrame();

	const Clock::time_point tick_end_time= Clock::now();
	const auto frame_dt= tick_end_time - tick_start_time;

	const float max_fps= 200.0f;

	const std::chrono::milliseconds min_frame_duration(uint32_t(1000.0f / max_fps));
	if(frame_dt <= min_frame_duration)
	{
		std::this_thread::sleep_for(min_frame_duration - frame_dt);
	}

	return false;
}

} // namespace Armed
