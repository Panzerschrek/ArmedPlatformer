#pragma once
#include "Assert.hpp"
#include <cstdint>
#include <vector>

namespace Armed
{

enum class TileId : uint8_t
{
	Air,
	BasicWall,
};

class TilesMap
{
public:
	explicit TilesMap(const char* data);

	uint32_t GetSizeX() const { return size_[0]; }
	uint32_t GetSizeY() const { return size_[1]; }

	TileId GetTile(const uint32_t x, const uint32_t y) const
	{
		ARMED_ASSERT(x < size_[0]);
		ARMED_ASSERT(y < size_[1]);
		return data_[x + y * size_[0]];
	}

private:
	const uint32_t size_[2];
	const std::vector<TileId> data_;
};

// Map parsing.

// Do not set values of these enums manually! Add new values at end!

enum class MonsterId
{
	Biter,
	Gunner,
	Bomber,
};

enum class PowerUpId
{
	SmallHealth,
};

struct MonsterInfo
{
	MonsterId id{};
	uint32_t pos[2]{};
};

struct PowerUpInfo
{
	PowerUpId id{};
	uint32_t pos[2]{};
};

struct MapObjectsData
{
	std::vector<MonsterInfo> monsters;
	std::vector<PowerUpInfo> power_ups;
};

MapObjectsData ExtractMapObjects(const char* map_data);

} // namespace Armed
