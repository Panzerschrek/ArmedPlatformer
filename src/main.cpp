#include "Log.hpp"
#include "Host.hpp"

namespace Armed
{

extern "C" int main()
{
	Host host;
	while(!host.Loop()){}
	return 0;
}

} // namespace Armed
