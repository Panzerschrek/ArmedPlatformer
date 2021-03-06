#pragma once
#include "SoundOut.hpp"
#include <array>

namespace Armed
{

enum class SoundId
{
	Explosion,
	Shot,
	PickUp,
	Bite,
	MonsterDeath,
	PlayerSpawn,
	MapEndMelody,
	NumSounds,
};

class SoundProcessor
{
public:
	SoundProcessor(SoundOut& sound_out);

	void MakeSound(SoundId sound_id);
	void StopAllSounds();

private:
	SoundOut& sound_out_;
	std::array<SoundData, size_t(SoundId::NumSounds)> sounds_;
};

} // namespace Armed
