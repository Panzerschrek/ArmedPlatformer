#include "Log.hpp"
#include "Host.hpp"

namespace Armed
{

extern "C" int main()
{
	try
	{
		Host host;
		while(!host.Loop()){}
	}
	catch(const std::exception& ex)
	{
		Log::FatalError("Exception throwed: ", ex.what());
	}

	return 0;
}

} // namespace Armed
