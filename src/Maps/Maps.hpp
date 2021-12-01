#pragma once
#include "Test.hpp"
#include "00.hpp"
#include "01.hpp"

namespace Armed
{

namespace Maps
{

// TODO - provide list of real maps.
const MapDescription maps_list[]
{
	Map00::description,
	Map01::description,
	Test::description,
};

} // namespace Maps

} // namespace Armed
