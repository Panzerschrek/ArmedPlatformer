#include "Log.hpp"
#include "Host.hpp"
#include <SDL.h>

namespace Armed
{

extern "C" int main(const int argc, const char* argv[])
{
	ARMED_UNUSED(argc);
	ARMED_UNUSED(argv);

	Host host;
	while(!host.Loop()){}
	return 0;
}

} // namespace Armed
