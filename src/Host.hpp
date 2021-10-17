#pragma once
#include "SystemWindow.hpp"
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
	using Clock= std::chrono::steady_clock;

	SystemWindow system_window_;

	const Clock::time_point init_time_;
	Clock::time_point prev_tick_time_;
};

} // namespace Armed
