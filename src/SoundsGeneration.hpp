#pragma once
#include "SoundOut.hpp"

namespace Armed
{

SoundData GenSinWaveSound(uint32_t frequency, fixed16_t sin_wave_frequency, uint32_t periods);
SoundData GenShotSound(uint32_t frequency);

} // namespace Armed
