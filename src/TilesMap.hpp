#pragma once
#include "Assert.hpp"
#include "SaveLoadStreams.hpp"
#include <cstdint>
#include <vector>

namespace Armed
{

enum class TileId : uint8_t
{
	Air,
	BasicWall,

	Dirt,
	DirtWithGrass,
	DirtLower,
	PawedDirt,

	TechnoBlock,
	TechnoBlockLower,
	PawedTechnoBlock,

	Rock,
	RockTop,
	RockLower,

	Lava,
	Water,
	MapEnd,

	KeyDoor0= 64,
	KeyDoor1,
	KeyDoor2,
	KeyDoor3,
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

	void Save(SaveStream& stream);
	static TilesMap Load(LoadStream& stream);

private:
	TilesMap(uint32_t size_x, uint32_t size_y, std::vector<TileId> data);

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
	Boss,
};

enum class PowerUpId
{
	SmallHealth,
	LargeHealth,
	Health2,
	Health3,
	Ammo0,
	Ammo1,
	Ammo2,
	Ammo3,
	Key0,
	Key1,
	Key2,
	Key3,
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
