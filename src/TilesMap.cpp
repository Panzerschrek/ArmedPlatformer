#include "TilesMap.hpp"
#include "Assert.hpp"

namespace Armed
{

namespace
{

namespace MapParser
{

uint32_t CalculateSizeX(const char* const data)
{
	uint32_t min_width= ~0u;
	uint32_t max_width= 0;
	uint32_t cur_width= 0;
	for(const char* c= data; *c != '\0'; ++c)
	{
		if(*c == '\n')
		{
			min_width= std::min(min_width, cur_width);
			max_width= std::max(max_width, cur_width);
			cur_width = 0;
		}
		else
			++cur_width;
	}

	ARMED_ASSERT(min_width == max_width); // All lines must have same length.
	ARMED_ASSERT(cur_width == 0); // Must end with newline.
	return min_width;
}

uint32_t CalculateSizeY(const char* const data)
{
	uint32_t heigth= 0;
	for(const char* c= data; *c != '\0'; ++c)
		if(*c == '\n')
			++heigth;

	return heigth;
}

TileId TileForChar(const char c)
{
	switch(c)
	{
	case ' ':
	default:
		return TileId::Air;
	case '#':
		return TileId::BasicWall;
	};
}

std::vector<TileId> ParseData(const char* const data)
{
	std::vector<TileId> result;

	for(const char* c= data; *c != '\0'; ++c)
		if(*c != '\n')
			result.push_back(TileForChar(*c));

	return result;
}

} // namespace MapParser

} // namespace

TilesMap::TilesMap(const char* const data)
	: size_{ MapParser::CalculateSizeX(data), MapParser::CalculateSizeY(data) }, data_(MapParser::ParseData(data))
{}

MapObjectsData ExtractMapObjects(const char* const map_data)
{
	MapObjectsData map_objects;

	uint32_t x= 0, y= 0;
	for(const char* c= map_data; *c != '\0'; ++c)
	{
		if(*c == '\n')
		{
			++y;
			x= 0;
			continue;
		}

		if(*c >= 'm' && *c <= 'z')
		{
			MonsterInfo monster;
			monster.pos[0]= x;
			monster.pos[1]= y;
			monster.id= MonsterId((uint32_t(*c) - uint32_t('m')));
			map_objects.monsters.push_back(monster);
		}
		if(*c >= '0' && *c < '9')
		{
			PowerUpInfo powr_up;
			powr_up.pos[0]= x;
			powr_up.pos[1]= y;
			powr_up.id= PowerUpId((uint32_t(*c) - uint32_t('m')));
			map_objects.power_ups.push_back(powr_up);
		}

		++x;
	}

	return map_objects;
}

} // namespace Armed
