#include "World.hpp"
#include "Assert.hpp"

namespace Armed
{

World::World(const MapDescription& map_description)
	: map_(map_description.tiles_map_data)
	, player_(map_description.player_spawn[0], map_description.player_spawn[1])
{
	const MapObjectsData map_objects= ExtractMapObjects(map_description.tiles_map_data);

	monsters_.reserve(map_objects.monsters.size());
	for(const MonsterInfo& monster_info : map_objects.monsters)
	{
		Monster monster;
		monster.spawn_tile_pos[0]= monster_info.pos[0];
		monster.spawn_tile_pos[1]= monster_info.pos[1];
		monster.pos[0]= IntToFixed16(int32_t(monster_info.pos[0])) + (g_fixed16_one >> 1);
		monster.pos[1]= IntToFixed16(int32_t(monster_info.pos[1])) + (g_fixed16_one >> 1);
		monster.move_dir= (monsters_.size() & 1) == 0 ? (+1) : (-1);
		monsters_.push_back(monster);
	}
}

void World::Tick(const InputState& input_state)
{
	ProcessPlayerPhysics(input_state);
	MoveMonsters();
}

void World::ProcessPlayerPhysics(const InputState& input_state)
{
	player_.Tick(input_state);
	player_.SetOnGround(false);

	// Check for collision against some geometry, correct player position, continue.
	for(uint32_t collision_number= 0; collision_number < 8; ++collision_number)
	{
		const fixed16vec2_t player_pos{ player_.GetPos()[0], player_.GetPos()[1] };

		const fixed16vec2_t player_bbox{ g_fixed16_one / 2, g_fixed16_one * 3 / 4 };

		const fixed16vec2_t bbox_min{ -player_bbox[0] / 2, -player_bbox[1] / 2 };
		const fixed16vec2_t bbox_max{ +player_bbox[0] / 2, +player_bbox[1] / 2 };

		const fixed16vec2_t bbox_transformed_min{ bbox_min[0] + player_pos[0], bbox_min[1] + player_pos[1] };
		const fixed16vec2_t bbox_transformed_max{ bbox_max[0] + player_pos[0], bbox_max[1] + player_pos[1] };

		const int32_t cell_min[2]{ Fixed16FloorToInt(bbox_transformed_min[0]), Fixed16FloorToInt(bbox_transformed_min[1]) };
		const int32_t cell_max[2]{ Fixed16FloorToInt(bbox_transformed_max[0]), Fixed16FloorToInt(bbox_transformed_max[1]) };

		for(int32_t y= std::max(0, cell_min[1] - 1); y <= std::min(cell_max[1] + 1, int32_t(map_.GetSizeY()) - 1); ++y)
		for(int32_t x= std::max(0, cell_min[0] - 1); x <= std::min(cell_max[0] + 1, int32_t(map_.GetSizeX()) - 1); ++x)
		{
			const TileId tile= map_.GetTile(uint32_t(x), uint32_t(y));
			switch(tile)
			{
			case TileId::Air:
				break;
			case TileId::BasicWall:
			{
				const fixed16vec2_t tile_bb_min{ IntToFixed16(x  ), IntToFixed16(y  ) };
				const fixed16vec2_t tile_bb_max{ IntToFixed16(x+1), IntToFixed16(y+1) };
				if( bbox_transformed_min[0] >= tile_bb_max[0] ||
					bbox_transformed_min[1] >= tile_bb_max[1] ||
					bbox_transformed_max[0] <= tile_bb_min[0] ||
					bbox_transformed_max[1] <= tile_bb_min[1] )
				{
					// Ok - does not collide.
				}
				else
				{
					const fixed16_t push_positive_x = tile_bb_max[0] - bbox_transformed_min[0];
					const fixed16_t push_positive_y = tile_bb_max[1] - bbox_transformed_min[1];
					const fixed16_t push_negative_x = tile_bb_min[0] - bbox_transformed_max[0];
					const fixed16_t push_negative_y = tile_bb_min[1] - bbox_transformed_max[1];

					const fixed16_t push_positive_x_abs = Fixed16Abs(push_positive_x);
					const fixed16_t push_positive_y_abs = Fixed16Abs(push_positive_y);
					const fixed16_t push_negative_x_abs = Fixed16Abs(push_negative_x);
					const fixed16_t push_negative_y_abs = Fixed16Abs(push_negative_y);

					// Search for smallest push distance. Check first Y pushes.
					fixed16vec2_t push_vec{0, 0};
					if( push_positive_y_abs <= push_positive_x_abs &&
						push_positive_y_abs <= push_negative_x_abs &&
						push_positive_y_abs <= push_negative_y_abs)
					{
						push_vec[1]= push_positive_y;
					} else
					if( push_negative_y_abs <= push_positive_x_abs &&
						push_negative_y_abs <= push_positive_y_abs &&
						push_negative_y_abs <= push_negative_x_abs)
					{
						player_.SetOnGround(true);
						push_vec[1]= push_negative_y;
					} else
					if( push_positive_x_abs <= push_positive_y_abs &&
						push_positive_x_abs <= push_negative_x_abs &&
						push_positive_x_abs <= push_negative_y_abs)
					{
						push_vec[0]= push_positive_x;
					} else
					/*
					if( push_negative_x_abs <= push_positive_x_abs &&
						push_negative_x_abs <= push_positive_y_abs &&
						push_negative_x_abs <= push_negative_y_abs)
					*/
					{
						push_vec[0]= push_negative_x;
					}

					player_.Push(push_vec);
					goto collsion_check_end;
				}
			}
				break;
			}
		}

		break; // No collision detected - skip all checks.
		collsion_check_end:;
	}
}

void World::MoveMonsters()
{
	for(Monster& monster : monsters_)
		MoveMonster(monster);
}

void World::MoveMonster(Monster& monster)
{
	const fixed16_t c_speed= g_fixed16_one / 64;
	const fixed16_t c_width= g_fixed16_one * 5 / 8;

	fixed16vec2_t new_pos{ monster.pos[0], monster.pos[1] };
	new_pos[0]+= monster.move_dir * c_speed;

	const int32_t min_x= Fixed16FloorToInt(new_pos[0] - c_width / 2);
	const int32_t max_x= Fixed16FloorToInt(new_pos[0] + c_width / 2);
	const int32_t y= Fixed16FloorToInt(new_pos[1]);
	bool can_move= true;
	for(int32_t x= std::max(0, min_x); x <= std::min(max_x, int32_t(map_.GetSizeX()) - 1); ++x)
	{
		if(map_.GetTile(uint32_t(x), uint32_t(y)) != TileId::Air)
			can_move= false;
		if(map_.GetTile(uint32_t(x), uint32_t(std::min(y + 1, int32_t(map_.GetSizeY()) - 1))) == TileId::Air)
			can_move= false;
	}

	// TODO - check for collisions agains other monsters and player.

	if(!can_move)
		monster.move_dir= -monster.move_dir;
	else
	{
		monster.pos[0]= new_pos[0];
		monster.pos[1]= new_pos[1];
	}
}

} // namespace Armed
