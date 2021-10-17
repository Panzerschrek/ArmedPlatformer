#include "Log.hpp"
#include <SDL_messagebox.h>

namespace Armed
{

Log::LogCallback Log::log_callback_;
std::ofstream Log::log_file_{ "Sazava_log.log" };

void Log::SetLogCallback(LogCallback callback)
{
	log_callback_= std::move(callback);
}

void Log::ShowFatalMessageBox(const std::string& error_message)
{
	SDL_ShowSimpleMessageBox(
		SDL_MESSAGEBOX_ERROR,
		"Fatal error",
		error_message.c_str(),
		nullptr );
}

} // namespace Armed
