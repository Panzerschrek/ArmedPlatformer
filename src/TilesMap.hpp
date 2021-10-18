#pragma once
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
		return data_[x + y * size_[0]];
	}

private:
	const uint32_t size_[2];
	const std::vector<TileId> data_;
};

} // namespace Armed
