#include "World.hpp"
#include "Assert.hpp"
#include <optional>

namespace Armed
{

namespace
{

const fixed16_t c_player_width= g_fixed16_one / 2;
const fixed16_t c_player_heigth= g_fixed16_one * 3 / 4;
const fixed16_t c_monster_width= g_fixed16_one * 5 / 8;
const fixed16_t c_monster_height= g_fixed16_one * 6 / 8;

const int32_t c_monster_max_distance_to_spawn= 6;
const fixed16_t c_player_monster_distance_for_chase_start= g_fixed16_one * c_monster_max_distance_to_spawn;

const World::TickT c_attack_frequency= World::c_update_frequency / 1;
const uint32_t c_melee_attack_min_damage= 20;
const uint32_t c_melee_attack_max_damage= 30;

std::optional<fixed16vec2_t> ProcessPlayerCollsion(
	const fixed16vec2_t& player_bb_min,
	const fixed16vec2_t& player_bb_max,
	const fixed16vec2_t& bb_min,
	const fixed16vec2_t& bb_max)
{
	if( player_bb_min[0] >= bb_max[0] ||
		player_bb_min[1] >= bb_max[1] ||
		player_bb_max[0] <= bb_min[0] ||
		player_bb_max[1] <= bb_min[1] )
		return std::nullopt; // No collision

	const fixed16_t push_positive_x = bb_max[0] - player_bb_min[0];
	const fixed16_t push_positive_y = bb_max[1] - player_bb_min[1];
	const fixed16_t push_negative_x = bb_min[0] - player_bb_max[0];
	const fixed16_t push_negative_y = bb_min[1] - player_bb_max[1];

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

	return push_vec;
}

} // namespace

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

void World::Tick(const InputState& input_state, const fixed16vec2_t& aim_vec)
{
	ProcessShootRequest(player_.Tick(input_state, aim_vec));
	ProcessPlayerPhysics();
	MoveMonsters();
	MoveProjectiles();
	++current_tick_;
}

void World::ProcessPlayerPhysics()
{
	player_.SetOnGround(false);

	// Check for collision against some geometry, correct player position, continue.
	for(uint32_t collision_number= 0; collision_number < 8; ++collision_number)
	{
		const fixed16vec2_t player_pos{ player_.GetPos()[0], player_.GetPos()[1] };

		const fixed16vec2_t bbox_min{ -c_player_width / 2, -c_player_heigth / 2 };
		const fixed16vec2_t bbox_max{ +c_player_width / 2, +c_player_heigth / 2 };

		const fixed16vec2_t bbox_transformed_min{ bbox_min[0] + player_pos[0], bbox_min[1] + player_pos[1] };
		const fixed16vec2_t bbox_transformed_max{ bbox_max[0] + player_pos[0], bbox_max[1] + player_pos[1] };

		// Check for collisions with geometry.
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
				if(const auto push_vec= ProcessPlayerCollsion(bbox_transformed_min, bbox_transformed_max, tile_bb_min, tile_bb_max))
				{
					player_.Push(*push_vec);
					if((*push_vec)[1] < 0)
						player_.SetOnGround(true);
					goto collsion_check_end;
				}
			}
				break;
			}
		}

		// Check for collisions against monsters.
		for(const Monster& monster : monsters_)
		{
			const fixed16vec2_t monster_bb_min{monster.pos[0] - c_monster_width / 2, monster.pos[1] - c_monster_height / 2};
			const fixed16vec2_t monster_bb_max{monster.pos[0] + c_monster_width / 2, monster.pos[1] + c_monster_height / 2};
			if(const auto push_vec= ProcessPlayerCollsion(bbox_transformed_min, bbox_transformed_max, monster_bb_min, monster_bb_max))
			{
				player_.Push(*push_vec);
				if((*push_vec)[1] < 0)
					player_.SetOnGround(true);
				goto collsion_check_end;
			}
		}

		break; // No collision detected - skip all checks.
		collsion_check_end:;
	}
}

void World::ProcessShootRequest(const Player::ShootRequestKind shoot_request)
{
	if(shoot_request == Player::ShootRequestKind::None)
		return;

	const fixed16_t c_vel= g_fixed16_one / 4;
	const fixed16vec2_t& aim_vec= player_.GetAimNormal();

	Projectile projectile;
	projectile.owner_kind= Projectile::OwnerKind::Player;
	projectile.pos= player_.GetPos();
	projectile.vel= {Fixed16Mul(aim_vec[0], c_vel), Fixed16Mul(aim_vec[1], c_vel)};
	projectiles_.push_back(projectile);
}

void World::MoveMonsters()
{
	for(Monster& monster : monsters_)
		MoveMonster(monster);
}

void World::MoveMonster(Monster& monster)
{
	const fixed16_t c_speed= g_fixed16_one / 64;

	fixed16vec2_t new_pos{ monster.pos[0], monster.pos[1] };
	new_pos[0]+= monster.move_dir * c_speed;

	const fixed16vec2_t bb_min{new_pos[0] - c_monster_width / 2, new_pos[1] - c_monster_height / 2};
	const fixed16vec2_t bb_max{new_pos[0] + c_monster_width / 2, new_pos[1] + c_monster_height / 2};

	bool can_move= true;

	// Check for possibility to move.
	{
		const int32_t min_x= Fixed16FloorToInt(bb_min[0]);
		const int32_t max_x= Fixed16FloorToInt(bb_max[0]);
		const int32_t y= Fixed16FloorToInt(new_pos[1]);

		for(int32_t x= std::max(0, min_x); x <= std::min(max_x, int32_t(map_.GetSizeX()) - 1); ++x)
		{
			if(map_.GetTile(uint32_t(x), uint32_t(y)) != TileId::Air)
				can_move= false;
			if(map_.GetTile(uint32_t(x), uint32_t(std::min(y + 1, int32_t(map_.GetSizeY()) - 1))) == TileId::Air)
				can_move= false;
		}
	}

	// Check collisions agains other monsters.
	for(const Monster& other_monster : monsters_)
	{
		if(&other_monster == &monster)
			continue;

		const fixed16vec2_t other_bb_min{other_monster.pos[0] - c_monster_width / 2, other_monster.pos[1] - c_monster_height / 2};
		const fixed16vec2_t other_bb_max{other_monster.pos[0] + c_monster_width / 2, other_monster.pos[1] + c_monster_height / 2};

		if( other_bb_min[0] >= bb_max[0] ||
			other_bb_min[1] >= bb_max[1] ||
			other_bb_max[0] <= bb_min[0] ||
			other_bb_max[1] <= bb_min[1])
			continue; // No collision.

		can_move= false;
	}

	{ // Try to attack player.
		const fixed16vec2_t player_bb_min{ -c_player_width / 2 + player_.GetPos()[0], -c_player_heigth / 2 + player_.GetPos()[1] };
		const fixed16vec2_t player_bb_max{ +c_player_width / 2 + player_.GetPos()[0], +c_player_heigth / 2 + player_.GetPos()[1] };

		if( player_bb_min[0] >= bb_max[0] ||
			player_bb_min[1] >= bb_max[1] ||
			player_bb_max[0] <= bb_min[0] ||
			player_bb_max[1] <= bb_min[1])
		{} // No collision.
		else
		{
			if(current_tick_ - monster.last_attack_tick >= c_attack_frequency)
			{
				const uint32_t damage= rand_.RandValue(c_melee_attack_min_damage, c_melee_attack_max_damage +1);
				player_.Hit(int32_t(damage));
				monster.last_attack_tick= current_tick_;
			}
		}
	}

	bool is_too_far_from_spawn_point= false;
	{ // Check for distance from spawn point.
		const int32_t signed_distance_to_spawn= Fixed16RoundToInt(new_pos[0]) - int32_t(monster.spawn_tile_pos[0]);
		if(std::abs(signed_distance_to_spawn) > c_monster_max_distance_to_spawn &&
			signed_distance_to_spawn * monster.move_dir > 0)
			is_too_far_from_spawn_point= true;
	}

	bool should_change_direction_towards_player= false;
	bool should_face_player= false;
	{ // Check for distance to player. Change direction if player is too close - stop patrooling, start moving towards player.
		const fixed16vec2_t dir_to_player{player_.GetPos()[0] - new_pos[0], player_.GetPos()[1] - new_pos[1]};
		if(std::abs(dir_to_player[1]) < g_fixed16_one * 2)
		{
			fixed16_t abs_dist= std::abs(dir_to_player[0]);
			// Add some value to esitamed distance based on direction of view. This is needed to prevent monster changing move direction in corner cases.
			const fixed16_t dir_dir_dot= monster.move_dir * dir_to_player[0]; // int * fixed16_t
			abs_dist+= dir_dir_dot > 0 ? (+g_fixed16_one/2) : (-g_fixed16_one/2);
			if(abs_dist < c_player_monster_distance_for_chase_start)
			{
				should_face_player= true;
				if(dir_dir_dot < 0)
					should_change_direction_towards_player= true;
			}
		}
	}

	if(should_face_player)
	{
		if(should_change_direction_towards_player)
			monster.move_dir= -monster.move_dir;
		else if(can_move)
			monster.pos= new_pos;
	}
	else if(is_too_far_from_spawn_point || !can_move)
		monster.move_dir= -monster.move_dir;
	else if(can_move)
		monster.pos= new_pos;
}

void World::MoveProjectiles()
{
	for(size_t i= 0; i < projectiles_.size();)
	{
		Projectile& projectile= projectiles_[i];
		if(!MoveProjectile(projectile))
		{
			if(&projectile != &projectiles_.back())
				projectile= projectiles_.back();
			projectiles_.pop_back();
		}
		else
			++i;
	}
}

bool World::MoveProjectile(Projectile& projectile)
{
	projectile.pos[0]+= projectile.vel[0];
	projectile.pos[1]+= projectile.vel[1];

	if( Fixed16CeilToInt(projectile.pos[0]) >= int32_t(map_.GetSizeX()) ||
		Fixed16CeilToInt(projectile.pos[0]) < 0 ||
		Fixed16CeilToInt(projectile.pos[1]) >= int32_t(map_.GetSizeY()) ||
		Fixed16CeilToInt(projectile.pos[1]) < 0)
		return false;

	const fixed16_t c_projectile_width = g_fixed16_one / 8;
	const fixed16_t c_projectile_height= g_fixed16_one / 8;
	const fixed16vec2_t bb_min{projectile.pos[0] - c_projectile_width / 2, projectile.pos[1] - c_projectile_height / 2};
	const fixed16vec2_t bb_max{projectile.pos[0] + c_projectile_width / 2, projectile.pos[1] + c_projectile_height / 2};

	// Check for collisions with geometry.
	const int32_t cell_min[2]{ Fixed16FloorToInt(bb_min[0]), Fixed16FloorToInt(bb_min[1]) };
	const int32_t cell_max[2]{ Fixed16FloorToInt(bb_max[0]), Fixed16FloorToInt(bb_max[1]) };

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
				if( tile_bb_min[0] >= bb_max[0] ||
					tile_bb_min[1] >= bb_max[1] ||
					tile_bb_max[0] <= bb_min[0] ||
					tile_bb_max[1] <= bb_min[1] )
				{} // No collision
				else
				{
					// TODO - generate explosion effect, sounds, etc.
					return false;
				}
			}
			break;
		}
	}

	if(projectile.owner_kind != Projectile::OwnerKind::Monster)
	{
		// Check for collisions against monsters.
		for(const Monster& monster : monsters_)
		{
			const fixed16vec2_t monster_bb_min{monster.pos[0] - c_monster_width / 2, monster.pos[1] - c_monster_height / 2};
			const fixed16vec2_t monster_bb_max{monster.pos[0] + c_monster_width / 2, monster.pos[1] + c_monster_height / 2};
			if( monster_bb_min[0] >= bb_max[0] ||
				monster_bb_min[1] >= bb_max[1] ||
				monster_bb_max[0] <= bb_min[0] ||
				monster_bb_max[1] <= bb_min[1] )
			{} // No collision
			else
			{
				// TODO - decrease monster health.
				return false;
			}
		}
	}
	if(projectile.owner_kind != Projectile::OwnerKind::Player)
	{
		const fixed16vec2_t player_bb_min{ -c_player_width / 2 + player_.GetPos()[0], -c_player_heigth / 2 + player_.GetPos()[1] };
		const fixed16vec2_t player_bb_max{ +c_player_width / 2 + player_.GetPos()[0], +c_player_heigth / 2 + player_.GetPos()[1] };

		if( player_bb_min[0] >= bb_max[0] ||
			player_bb_min[1] >= bb_max[1] ||
			player_bb_max[0] <= bb_min[0] ||
			player_bb_max[1] <= bb_min[1])
		{} // No collision.
		else
		{
			// TODO - hit player.
			return false;
		}
	}

	return true;
}


} // namespace Armed
