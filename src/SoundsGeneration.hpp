#pragma once
#include "SoundOut.hpp"

namespace Armed
{

SoundData GenSinWaveSound(uint32_t sample_rate, fixed16_t sin_wave_frequency, uint32_t periods);
SoundData GenExplosionSound(uint32_t sample_rate);
SoundData GenShotSound(uint32_t sample_rate);
SoundData GenPickUpSound(uint32_t sample_rate);
SoundData GenMapEndMelody(uint32_t sample_rate);

} // namespace Armed
