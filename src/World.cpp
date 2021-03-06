#include "World.hpp"
#include "Assert.hpp"
#include <optional>

namespace Armed
{

namespace
{

const fixed16_t c_player_width= g_fixed16_one * 3 / 2;
const fixed16_t c_player_heigth= g_fixed16_one * 7 / 4;

const fixed16_t c_platform_width= g_fixed16_one * 3 / 2;
const fixed16_t c_platform_height= g_fixed16_one * 3 / 4;

const int32_t c_monster_max_distance_to_spawn= 6;
const fixed16_t c_player_monster_distance_for_chase_start= g_fixed16_one * c_monster_max_distance_to_spawn;

const fixed16_t c_bomber_attack_distance= g_fixed16_one * 5;

const World::TickT c_attack_frequency= World::c_update_frequency / 1;
const uint32_t c_melee_attack_min_damage= 20;
const uint32_t c_melee_attack_max_damage= 30;
const int32_t c_small_health= 25;
const int32_t c_large_health= 50;

const int32_t c_bullet_damage= 10;
const int32_t c_grenade_damage= 10;
const int32_t c_bomb_damage= 0;

const int32_t c_grenade_splash_damage= 30;
const int32_t c_bomb_splash_damage= 40;

const int32_t c_lava_damage= 3;

const fixed16_t c_platform_speed= g_fixed16_one / 32;

const fixed16_t c_bullet_speed= g_fixed16_one / 3;
const fixed16_t c_grenade_speed= g_fixed16_one / 5;

fixed16vec2_t GetMonsterSize(const MonsterId monster_id)
{
	switch(monster_id)
	{
	case MonsterId::Biter: return {g_fixed16_one * 3 / 2, g_fixed16_one * 7 / 4};
	case MonsterId::Gunner: return {g_fixed16_one * 7 / 4, g_fixed16_one * 7 / 4};
	case MonsterId::Bomber: return {g_fixed16_one * 2, g_fixed16_one * 5 / 4};
	case MonsterId::Boss: return {g_fixed16_one * 3, g_fixed16_one * 3};
	};
	ARMED_ASSERT(false);
	return {0, 0};
}

int32_t GetMonsterHealth(const MonsterId monster_id)
{
	switch(monster_id)
	{
	case MonsterId::Biter: return 20;
	case MonsterId::Gunner: return 20;
	case MonsterId::Bomber: return 20;
	case MonsterId::Boss: return World::c_boss_health;
	};
	ARMED_ASSERT(false);
	return 0;
}

int32_t GetProjectileDirectDamage(const World::Projectile::Kind kind)
{
	using Kind= World::Projectile::Kind;
	switch(kind)
	{
	case Kind::Bullet: return c_bullet_damage;
	case Kind::Grenade: return c_grenade_damage;
	case Kind::Bomb: return c_bomb_damage;
	}

	ARMED_ASSERT(false);
	return c_bullet_damage;
}

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

World::World(const MapDescription& map_description, SoundProcessor& sound_processor)
	: sound_processor_(sound_processor)
	, map_(map_description.tiles_map_data)
	, player_(map_description.player_spawn[0], map_description.player_spawn[1])
{
	const MapObjectsData map_objects= ExtractMapObjects(map_description.tiles_map_data);

	platforms_.reserve(map_description.platforms.size());
	for(const PlatformDescription& platform_info : map_description.platforms)
	{
		Platform platform;
		platform.points[0]= {IntToFixed16(platform_info.points[0][0]), IntToFixed16(platform_info.points[0][1])};
		platform.points[1]= {IntToFixed16(platform_info.points[1][0]), IntToFixed16(platform_info.points[1][1])};
		platform.pos= platform.points[0];

		if(platform.points[0][0] < platform.points[1][0])
			platform.vel= {+c_platform_speed, 0};
		if(platform.points[0][0] > platform.points[1][0])
			platform.vel= {-c_platform_speed, 0};

		if(platform.points[0][1] < platform.points[1][1])
			platform.vel= {0, +c_platform_speed};
		if(platform.points[0][1] > platform.points[1][1])
			platform.vel= {0, -c_platform_speed};

		platforms_.push_back(platform);
	}

	monsters_.reserve(map_objects.monsters.size());
	for(const MonsterInfo& monster_info : map_objects.monsters)
	{
		Monster monster;
		monster.id= monster_info.id;
		monster.health= GetMonsterHealth(monster_info.id);
		monster.spawn_tile_pos[0]= monster_info.pos[0];
		monster.spawn_tile_pos[1]= monster_info.pos[1];
		monster.pos[0]= IntToFixed16(int32_t(monster_info.pos[0])) + g_fixed16_one / 2;
		monster.pos[1]= IntToFixed16(int32_t(monster_info.pos[1])) + g_fixed16_one - GetMonsterSize(monster.id)[1] / 2 - 1;
		monster.move_dir= (monsters_.size() & 1) == 0 ? (+1) : (-1);
		monsters_.push_back(monster);
	}

	power_ups_.reserve(map_objects.power_ups.size());
	for(const PowerUpInfo& power_up_info : map_objects.power_ups)
	{
		PowerUp power_up;
		power_up.id= power_up_info.id;
		power_up.pos= {IntToFixed16(int32_t(power_up_info.pos[0])) + (g_fixed16_one >> 1), IntToFixed16(int32_t(power_up_info.pos[1])) + (g_fixed16_one >> 1)};
		power_up.picked_up= false;
		power_ups_.push_back(power_up);
	}

	sound_processor_.MakeSound(SoundId::PlayerSpawn);
}

void World::Tick(const InputState& input_state, const fixed16vec2_t& aim_vec)
{
	if(map_end_reach_time_ == std::nullopt)
		ProcessShootRequest(player_.Tick(input_state, aim_vec));
	else if((current_tick_ - *map_end_reach_time_) >= 5 * c_update_frequency)
	{
		if(input_state.keyboard[size_t(SystemEventTypes::KeyCode::Space)] || input_state.mouse.any())
			trigger_map_change_= true;
	}

	ProcessPlayerPhysics();
	PickUpPowerUps();
	MovePlatforms();
	MoveMonsters();
	MoveProjectiles();
	UpdateExplosions();
	++current_tick_;
}

void World::Save(SaveStream& stream)
{
	stream.Write(rand_.GetInnerState());
	map_.Save(stream);
	player_.Save(stream);

	stream.Write(platforms_.size());
	for(const Platform& platform : platforms_)
	{
		stream.Write(platform.points);
		stream.Write(platform.pos);
		stream.Write(platform.vel);
	}

	stream.Write(monsters_.size());
	for(const Monster& monster : monsters_)
	{
		stream.Write(size_t(monster.id));
		stream.Write(monster.spawn_tile_pos);
		stream.Write(monster.pos);
		stream.Write(monster.move_dir);
		stream.Write(monster.last_attack_tick);
		stream.Write(monster.health);
	}

	stream.Write(power_ups_.size());
	for(const PowerUp& power_up : power_ups_)
	{
		stream.Write(size_t(power_up.id));
		stream.Write(power_up.pos);
		stream.Write(power_up.picked_up);
	}

	stream.Write(projectiles_.size());
	for(const Projectile& projectile : projectiles_)
	{
		stream.Write(size_t(projectile.owner_kind));
		stream.Write(size_t(projectile.kind));
		stream.Write(projectile.pos);
		stream.Write(projectile.vel);
	}

	stream.Write(explosions_.size());
	for(const Explosion& explosion : explosions_)
	{
		stream.Write(explosion.pos);
		stream.Write(explosion.age);
	}

	stream.Write(current_tick_);
	stream.Write(map_end_reach_time_);
	stream.Write(trigger_map_change_);
}

World World::Load(SoundProcessor& sound_processor, LoadStream& stream)
{
	uint32_t rand_state;
	stream.Read(rand_state);

	Rand rand;
	rand.SetInnerState(rand_state);

	TilesMap map= TilesMap::Load(stream);
	Player player= Player::Load(stream);

	World w(sound_processor, std::move(rand), std::move(map), std::move(player));

	w.LoadImpl(stream);
	return w;
}

World::World(SoundProcessor& sound_processor, Rand rand, TilesMap map, Player player)
	: sound_processor_(sound_processor), rand_(std::move(rand)), map_(std::move(map)), player_(std::move(player))
{
}

void World::LoadImpl(LoadStream& stream)
{
	const auto read_vec_size=
	[&](auto& vec)
	{
		size_t size= 0;
		stream.Read(size);
		vec.resize(size);
	};

	read_vec_size(platforms_);
	for(Platform& platform : platforms_)
	{
		stream.Read(platform.points);
		stream.Read(platform.pos);
		stream.Read(platform.vel);
	}

	read_vec_size(monsters_);
	for(Monster& monster : monsters_)
	{
		size_t monster_id;
		stream.Read(monster_id);
		monster.id= MonsterId(monster_id);
		stream.Read(monster.spawn_tile_pos);
		stream.Read(monster.pos);
		stream.Read(monster.move_dir);
		stream.Read(monster.last_attack_tick);
		stream.Read(monster.health);
	}

	read_vec_size(power_ups_);
	for(PowerUp& power_up : power_ups_)
	{
		size_t power_up_id;
		stream.Read(power_up_id);
		power_up.id= PowerUpId(power_up_id);
		stream.Read(power_up.pos);
		stream.Read(power_up.picked_up);
	}

	read_vec_size(projectiles_);
	for(Projectile& projectile : projectiles_)
	{
		size_t owner_kind;
		stream.Read(owner_kind);
		projectile.owner_kind= Projectile::OwnerKind(owner_kind);
		size_t kind;
		stream.Read(kind);
		projectile.kind= Projectile::Kind(kind);
		stream.Read(projectile.pos);
		stream.Read(projectile.vel);
	}

	read_vec_size(explosions_);
	for(Explosion& explosion : explosions_)
	{
		stream.Read(explosion.pos);
		stream.Read(explosion.age);
	}

	stream.Read(current_tick_);
	stream.Read(map_end_reach_time_);
	stream.Read(trigger_map_change_);
}

void World::ProcessPlayerPhysics()
{
	player_.SetOnGround(false);
	player_.SetInLiquid(false);

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
			const fixed16vec2_t tile_bb_min{ IntToFixed16(x  ), IntToFixed16(y  ) };
			const fixed16vec2_t tile_bb_max{ IntToFixed16(x+1), IntToFixed16(y+1) };
			switch(tile)
			{
			case TileId::Air:
				break;
			case TileId::MapEnd:
				if( player_pos[0] >= tile_bb_min[0] && player_pos[0] < tile_bb_max[0] &&
					player_pos[1] >= tile_bb_min[1] && player_pos[1] < tile_bb_max[1])
					TriggerMapEnd();
				break;
			case TileId::BasicWall:
			case TileId::Dirt:
			case TileId::DirtWithGrass:
			case TileId::DirtLower:
			case TileId::PawedDirt:
			case TileId::TechnoBlock:
			case TileId::TechnoBlockLower:
			case TileId::PawedTechnoBlock:
			case TileId::Rock:
			case TileId::RockTop:
			case TileId::RockLower:
				if(const auto push_vec= ProcessPlayerCollsion(bbox_transformed_min, bbox_transformed_max, tile_bb_min, tile_bb_max))
				{
					player_.Push(*push_vec);
					if((*push_vec)[1] < 0)
						player_.SetOnGround(true);
					goto collsion_check_end;
				}
				break;
			case TileId::KeyDoor0:
			case TileId::KeyDoor1:
			case TileId::KeyDoor2:
			case TileId::KeyDoor3:
				if(!player_.HasKey(size_t(tile) - size_t(TileId::KeyDoor0)))
				{
					if(const auto push_vec= ProcessPlayerCollsion(bbox_transformed_min, bbox_transformed_max, tile_bb_min, tile_bb_max))
					{
						player_.Push(*push_vec);
						if((*push_vec)[1] < 0)
							player_.SetOnGround(true);
						goto collsion_check_end;
					}
				}
			case TileId::Water:
				if( player_pos[0] >= tile_bb_min[0] && player_pos[0] < tile_bb_max[0] &&
					player_pos[1] >= tile_bb_min[1] && player_pos[1] < tile_bb_max[1])
					player_.SetInLiquid(true);
				break;
			case TileId::Lava:
				if( player_pos[0] >= tile_bb_min[0] && player_pos[0] < tile_bb_max[0] &&
					player_pos[1] >= tile_bb_min[1] && player_pos[1] < tile_bb_max[1])
				{
					player_.SetInLiquid(true);
					if(current_tick_ % 12 == 0 && player_.GetHealth() >= 0)
					{
						player_.Hit(c_lava_damage);
						sound_processor_.MakeSound(SoundId::Bite);
					}
				}
				break;
			}
		}

		// Check for collisions against monsters.
		for(const Monster& monster : monsters_)
		{
			if(monster.health <= 0)
				continue;

			const fixed16vec2_t monster_size= GetMonsterSize(monster.id);
			const fixed16vec2_t monster_bb_min{monster.pos[0] - monster_size[0] / 2, monster.pos[1] - monster_size[1] / 2};
			const fixed16vec2_t monster_bb_max{monster.pos[0] + monster_size[0] / 2, monster.pos[1] + monster_size[1] / 2};
			if(const auto push_vec= ProcessPlayerCollsion(bbox_transformed_min, bbox_transformed_max, monster_bb_min, monster_bb_max))
			{
				player_.Push(*push_vec);
				if((*push_vec)[1] < 0)
					player_.SetOnGround(true);
				goto collsion_check_end;
			}
		}

		// Process interaction with platforms.
		for(const Platform& platform : platforms_)
		{
			const fixed16vec2_t platform_bb_min{platform.pos[0] - c_platform_width / 2, platform.pos[1] - c_platform_height / 2};
			const fixed16vec2_t platform_bb_max{platform.pos[0] + c_platform_width / 2, platform.pos[1] + c_platform_height / 2};
			if(const auto push_vec= ProcessPlayerCollsion(bbox_transformed_min, bbox_transformed_max, platform_bb_min, platform_bb_max))
			{
				player_.Push(*push_vec);
				if((*push_vec)[1] < 0)
				{
					player_.AccelerateByMovingPlatform(platform.vel);
					player_.SetOnGround(true);
				}
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

	const fixed16_t vel= shoot_request == Player::ShootRequestKind::GrenadeLauncher ? c_grenade_speed : c_bullet_speed;

	const fixed16vec2_t& aim_vec= player_.GetAimNormal();

	Projectile projectile;
	projectile.owner_kind= Projectile::OwnerKind::Player;
	projectile.pos= player_.GetPos();
	projectile.vel= {Fixed16Mul(aim_vec[0], vel), Fixed16Mul(aim_vec[1], vel)};
	projectile.kind= shoot_request == Player::ShootRequestKind::GrenadeLauncher ? Projectile::Kind::Grenade : Projectile::Kind::Bullet;
	projectiles_.push_back(projectile);

	sound_processor_.MakeSound(SoundId::Shot);
}

void World::PickUpPowerUps()
{
	const fixed16_t c_pick_up_distance= g_fixed16_one;

	const fixed16vec2_t player_pos= player_.GetPos();
	for(PowerUp& power_up : power_ups_)
	{
		if(power_up.picked_up)
			continue;

		// Use approximate distance to avoid overflow of square distance.
		const fixed16_t dist= Fixed16Abs(player_pos[0] - power_up.pos[0]) + Fixed16Abs(player_pos[1] - power_up.pos[1]);
		if(dist <= c_pick_up_distance)
		{
			switch(power_up.id)
			{
			case PowerUpId::SmallHealth:
				if(player_.GetHealth() < Player::c_max_health)
				{
					player_.Hit(-c_small_health);
					power_up.picked_up= true;
					sound_processor_.MakeSound(SoundId::PickUp);
				}
				break;
			case PowerUpId::LargeHealth:
				if(player_.GetHealth() < Player::c_max_health)
				{
					player_.Hit(-c_large_health);
					power_up.picked_up= true;
					sound_processor_.MakeSound(SoundId::PickUp);
				}
				break;
			case PowerUpId::Health2:
			case PowerUpId::Health3:
				break;
			case PowerUpId::Ammo0:
			case PowerUpId::Ammo1:
			case PowerUpId::Ammo2:
			case PowerUpId::Ammo3:
				{
					const size_t index= size_t(power_up.id) - size_t(PowerUpId::Ammo0);
					if(player_.GetAmmo(index) < Player::c_max_ammo[index])
					{
						player_.GiveAmmo(index, Player::c_max_ammo[index] / 2);
						power_up.picked_up= true;
						sound_processor_.MakeSound(SoundId::PickUp);
					}
				}
				break;
			case PowerUpId::Key0:
			case PowerUpId::Key1:
			case PowerUpId::Key2:
			case PowerUpId::Key3:
				{
					const size_t index= size_t(power_up.id) - size_t(PowerUpId::Key0);
					if(!player_.HasKey(index))
					{
						player_.GiveKey(index);
						power_up.picked_up= true;
						sound_processor_.MakeSound(SoundId::PickUp);
					}
				}
				break;
			};
		}

	}
}

void World::MovePlatforms()
{
	for(Platform& platform : platforms_)
		MovePlatform(platform);
}

void World::MovePlatform(Platform& platform)
{
	fixed16vec2_t new_pos= platform.pos;
	new_pos[0]+= platform.vel[0];
	new_pos[1]+= platform.vel[1];

	if( new_pos[0] > std::max(platform.points[0][0], platform.points[1][0]) ||
		new_pos[0] < std::min(platform.points[0][0], platform.points[1][0]))
		platform.vel[0]*= -1;
	else
	if( new_pos[1] > std::max(platform.points[0][1], platform.points[1][1]) ||
		new_pos[1] < std::min(platform.points[0][1], platform.points[1][1]))
		platform.vel[1]*= -1;
	else
		platform.pos= new_pos;
}

void World::MoveMonsters()
{
	for(Monster& monster : monsters_)
		MoveMonster(monster);
}

void World::MoveMonster(Monster& monster)
{
	if(monster.health <= 0)
		return;

	const fixed16_t c_speed= g_fixed16_one / 64;

	fixed16vec2_t new_pos{ monster.pos[0], monster.pos[1] };
	new_pos[0]+= monster.move_dir * c_speed;

	const fixed16vec2_t monster_size= GetMonsterSize(monster.id);
	const fixed16vec2_t bb_min{new_pos[0] - monster_size[0] / 2, new_pos[1] - monster_size[1] / 2};
	const fixed16vec2_t bb_max{new_pos[0] + monster_size[0] / 2, new_pos[1] + monster_size[1] / 2};

	bool can_move= true;

	// Check for possibility to move.
	{
		const int32_t min_x= Fixed16FloorToInt(bb_min[0]);
		const int32_t max_x= Fixed16FloorToInt(bb_max[0]);
		const int32_t min_y= Fixed16FloorToInt(bb_min[1]);
		const int32_t max_y= Fixed16FloorToInt(bb_max[1]);

		for(int32_t x= std::max(0, min_x); x <= std::min(max_x, int32_t(map_.GetSizeX()) - 1); ++x)
		{
			const auto lower_tile= map_.GetTile(uint32_t(x), uint32_t(std::min(max_y + 1, int32_t(map_.GetSizeY()) - 1)));
			if(monster.id != MonsterId::Bomber &&
				(lower_tile == TileId::Air || lower_tile == TileId::Water || lower_tile == TileId::Lava))
				can_move= false;

			for(int32_t y= std::max(0, min_y); y <= std::min(max_y, int32_t(map_.GetSizeY()) - 1); ++y)
			{
				if(map_.GetTile(uint32_t(x), uint32_t(y)) != TileId::Air)
					can_move= false;
			}
		}
	}

	// Check collisions agains other monsters.
	for(const Monster& other_monster : monsters_)
	{
		if(&other_monster == &monster)
			continue;

		const fixed16vec2_t other_monster_size= GetMonsterSize(other_monster.id);
		const fixed16vec2_t other_bb_min{other_monster.pos[0] - other_monster_size[0] / 2, other_monster.pos[1] - other_monster_size[1] / 2};
		const fixed16vec2_t other_bb_max{other_monster.pos[0] + other_monster_size[0] / 2, other_monster.pos[1] + other_monster_size[1] / 2};

		if( other_bb_min[0] >= bb_max[0] ||
			other_bb_min[1] >= bb_max[1] ||
			other_bb_max[0] <= bb_min[0] ||
			other_bb_max[1] <= bb_min[1])
			continue; // No collision.

		can_move= false;
	}

	if(monster.id == MonsterId::Biter)
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

				sound_processor_.MakeSound(SoundId::Bite);
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
	const fixed16vec2_t dir_to_player{player_.GetPos()[0] - new_pos[0], player_.GetPos()[1] - new_pos[1]};
	if(monster.id == MonsterId::Bomber)
	{
		if(std::abs(dir_to_player[0]) < c_bomber_attack_distance && dir_to_player[1] > 0 &&
			current_tick_ - monster.last_attack_tick >= c_attack_frequency)
		{
			Projectile projectile;
			projectile.owner_kind= Projectile::OwnerKind::Monster;
			projectile.kind= Projectile::Kind::Bomb;
			projectile.pos= monster.pos;
			projectile.vel= {fixed16_t(rand_.RandValue(Rand::RandResultType(g_fixed16_one / 64)) - g_fixed16_one / 128), g_fixed16_one / 64};
			projectiles_.push_back(projectile);
			monster.last_attack_tick= current_tick_;
		}
	}
	else
	{ // Check for distance to player. Change direction if player is too close - stop patrooling, start moving towards player.
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
				else if(monster.id == MonsterId::Gunner)
				{
					if(current_tick_ - monster.last_attack_tick >= c_attack_frequency)
					{
						Projectile projectile;
						projectile.owner_kind= Projectile::OwnerKind::Monster;
						projectile.pos= monster.pos;
						projectile.vel= {(dir_to_player[0] > 0 ? (+1) : (-1)) * c_bullet_speed, 0};
						projectiles_.push_back(projectile);
						monster.last_attack_tick= current_tick_;

						sound_processor_.MakeSound(SoundId::Shot);
					}
				}
				else if(monster.id == MonsterId::Boss)
				{
					if(current_tick_ - monster.last_attack_tick >= c_attack_frequency)
					{
						const fixed16_t vel= c_grenade_speed;

						fixed16vec2_t aim_vec{
							(dir_to_player[0] > 0 ? (+1) : (-1)) * g_fixed16_one,
							-fixed16_t(rand_.RandValue(Rand::RandResultType(0), Rand::RandResultType(g_fixed16_one)))};
						const auto aim_vec_len= Fixed16VecLen(aim_vec);
						for(size_t i= 0; i < 2; ++i)
							aim_vec[i]= Fixed16Div(aim_vec[i], aim_vec_len);

						Projectile projectile;
						projectile.owner_kind= Projectile::OwnerKind::Monster;
						projectile.pos= monster.pos;
						projectile.vel= {Fixed16Mul(aim_vec[0], vel), Fixed16Mul(aim_vec[1], vel)};
						projectile.kind= Projectile::Kind::Grenade;
						projectiles_.push_back(projectile);
						monster.last_attack_tick= current_tick_;

						sound_processor_.MakeSound(SoundId::Shot);
					}
				}
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
	const fixed16_t c_projectile_gravity= g_fixed16_one / 384;
	if(projectile.kind == Projectile::Kind::Grenade || projectile.kind == Projectile::Kind::Bomb)
		projectile.vel[1]+= c_projectile_gravity;

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
		case TileId::Water:
		case TileId::Lava:
		case TileId::MapEnd:
			break;
		// Block movement of projectiles by all doors.
		case TileId::KeyDoor0:
		case TileId::KeyDoor1:
		case TileId::KeyDoor2:
		case TileId::KeyDoor3:
		case TileId::BasicWall:
		case TileId::Dirt:
		case TileId::DirtWithGrass:
		case TileId::DirtLower:
		case TileId::PawedDirt:
		case TileId::TechnoBlock:
		case TileId::TechnoBlockLower:
		case TileId::PawedTechnoBlock:
		case TileId::Rock:
		case TileId::RockTop:
		case TileId::RockLower:
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
					ProcessProjectileHit(projectile);
					return false;
				}
			}
			break;
		}
	}

	if(projectile.owner_kind != Projectile::OwnerKind::Monster)
	{
		// Check for collisions against monsters.
		for(Monster& monster : monsters_)
		{
			if(monster.health <= 0)
				continue;

			const fixed16vec2_t monster_size= GetMonsterSize(monster.id);
			const fixed16vec2_t monster_bb_min{monster.pos[0] - monster_size[0] / 2, monster.pos[1] - monster_size[1] / 2};
			const fixed16vec2_t monster_bb_max{monster.pos[0] + monster_size[0] / 2, monster.pos[1] + monster_size[1] / 2};
			if( monster_bb_min[0] >= bb_max[0] ||
				monster_bb_min[1] >= bb_max[1] ||
				monster_bb_max[0] <= bb_min[0] ||
				monster_bb_max[1] <= bb_min[1] )
			{} // No collision
			else
			{
				monster.health-= GetProjectileDirectDamage(projectile.kind);
				ProcessProjectileHit(projectile);

				if(monster.health <= 0)
				{
					if(monster.id == MonsterId::Boss)
						TriggerMapEnd();

					sound_processor_.MakeSound(SoundId::MonsterDeath);
				}

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
			player_.Hit(GetProjectileDirectDamage(projectile.kind));
			ProcessProjectileHit(projectile);

			sound_processor_.MakeSound(SoundId::Bite);

			return false;
		}
	}

	// Check for collisions against platforms.
	for(const Platform& platform : platforms_)
	{
		const fixed16vec2_t platform_bb_min{platform.pos[0] - c_platform_width / 2, platform.pos[1] - c_platform_height / 2};
		const fixed16vec2_t platform_bb_max{platform.pos[0] + c_platform_width / 2, platform.pos[1] + c_platform_height / 2};
		if( platform_bb_min[0] >= bb_max[0] ||
			platform_bb_min[1] >= bb_max[1] ||
			platform_bb_max[0] <= bb_min[0] ||
			platform_bb_max[1] <= bb_min[1])
		{} // No collision.
		else
		{
			ProcessProjectileHit(projectile);
			return false;
		}
	}

	return true;
}

void World::ProcessProjectileHit(const Projectile& projectile)
{
	if(projectile.kind == Projectile::Kind::Grenade || projectile.kind == Projectile::Kind::Bomb)
	{
		const fixed16_t radius= projectile.kind == Projectile::Kind::Bomb ? g_fixed16_one * 2 : g_fixed16_one;
		const int32_t damage= projectile.kind == Projectile::Kind::Bomb ? c_bomb_splash_damage : c_grenade_splash_damage;
		ApplySplashDamage(projectile.owner_kind, projectile.pos, radius, damage);
		AddExplosion(projectile.pos);
	}
}

void World::ApplySplashDamage(const Projectile::OwnerKind owner_kind, const fixed16vec2_t& pos, const fixed16_t radius, const int32_t base_damage)
{
	const auto get_damage=
	[&](const fixed16vec2_t& bb_min, const fixed16vec2_t& bb_max)
	{
		fixed16vec2_t d{0, 0};
		for(size_t i= 0; i < 2; ++i)
		{
			if(pos[i] < bb_min[i])
				d[i]= bb_min[i] - pos[i];
			if(pos[i] > bb_max[i])
				d[i]= pos[i] - bb_max[i];
		}

		const fixed16_t dist= d[0] + d[1];
		if(dist < radius)
			return base_damage * (radius - dist) / radius;
		return 0;
	};

	if(owner_kind != Projectile::OwnerKind::Monster)
	{
		// Check for collisions against monsters.
		for(Monster& monster : monsters_)
		{
			if(monster.health <= 0)
				continue;

			const fixed16vec2_t monster_size= GetMonsterSize(monster.id);
			const fixed16vec2_t monster_bb_min{monster.pos[0] - monster_size[0] / 2, monster.pos[1] - monster_size[1] / 2};
			const fixed16vec2_t monster_bb_max{monster.pos[0] + monster_size[0] / 2, monster.pos[1] + monster_size[1] / 2};
			monster.health -= get_damage(monster_bb_min, monster_bb_max);

			if(monster.health <= 0)
				sound_processor_.MakeSound(SoundId::MonsterDeath);
		}
	}

	// Affect player even with its own projectiles.
	const fixed16vec2_t player_bb_min{ -c_player_width / 2 + player_.GetPos()[0], -c_player_heigth / 2 + player_.GetPos()[1] };
	const fixed16vec2_t player_bb_max{ +c_player_width / 2 + player_.GetPos()[0], +c_player_heigth / 2 + player_.GetPos()[1] };

	const auto player_damage= get_damage(player_bb_min, player_bb_max);
	if(player_damage > 0)
	{
		player_.Hit(player_damage);
		sound_processor_.MakeSound(SoundId::Bite);
	}
}

void World::AddExplosion(const fixed16vec2_t& pos)
{
	Explosion explosion;
	explosion.pos= pos;
	explosions_.push_back(explosion);

	sound_processor_.MakeSound(SoundId::Explosion);
}

void World::UpdateExplosions()
{
	for(size_t i= 0; i < explosions_.size();)
	{
		++explosions_[i].age;
		if(explosions_[i].age > Explosion::c_max_age)
		{
			if(i + 1 < explosions_.size())
				explosions_[i]= explosions_.back();
			explosions_.pop_back();
		}
		else
			++i;
	}
}

void World::TriggerMapEnd()
{
	if(map_end_reach_time_ == std::nullopt)
	{
		map_end_reach_time_= current_tick_;
		sound_processor_.MakeSound(SoundId::MapEndMelody);
	}
}

} // namespace Armed
