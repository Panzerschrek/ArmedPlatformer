#pragma once
#include "SoundOut.hpp"
#include <array>

namespace Armed
{

enum class SoundId
{
	Explosion,

	NumSounds,
};

class SoundProcessor
{
public:
	SoundProcessor(SoundOut& sound_out);

	void MakeSound(SoundId sound_id);

private:
	SoundOut& sound_out_;
	std::array<SoundData, size_t(SoundId::NumSounds)> sounds_;
};

} // namespace Armed
