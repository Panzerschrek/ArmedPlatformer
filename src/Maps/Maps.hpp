#pragma once
#include "Test.hpp"
#include "00.hpp"
#include "01.hpp"
#include "02.hpp"
#include "03.hpp"
#include "04.hpp"
#include "05.hpp"

namespace Armed
{

namespace Maps
{

const MapDescription maps_list[]
{
	Map00::description,
	Map01::description,
	Map02::description,
	Map03::description,
	Map04::description,
	Map05::description,
};

} // namespace Maps

} // namespace Armed
