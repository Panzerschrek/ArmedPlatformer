#include "SoundsGeneration.hpp"
#include "Rand.hpp"
#include <limits>

namespace Armed
{

namespace
{

const int32_t g_samle_scale= int32_t(std::numeric_limits<SampleType>::max());
const float g_two_pi = 2.0f * 3.1415926535f;

} // namespace

SoundData GenSinWaveSound(const uint32_t frequency, const fixed16_t sin_wave_frequency, const uint32_t periods)
{
	const uint32_t total_samples= uint32_t(uint64_t(frequency) * uint64_t(periods) * uint64_t(g_fixed16_one) / uint64_t(sin_wave_frequency));

	SoundData out_data;
	out_data.samples.resize(total_samples);

	const float scale_f= float(sin_wave_frequency) * g_two_pi / (float(frequency) * float(g_fixed16_one));
	const float sample_scale= float(g_samle_scale);
	for(uint32_t i= 0; i < total_samples; ++i)
		out_data.samples[i]= SampleType(std::sin(float(i) * scale_f) * sample_scale);

	return out_data;
}

SoundData GenExplosionSound(const uint32_t frequency)
{
	std::vector<int32_t> noise;
	noise.resize(2048);

	Rand rand;
	for(size_t i= 0; i < noise.size(); ++i)
		noise[i]= int32_t(rand.RandValue(0, 65536));

	const uint32_t total_samples= frequency;
	SoundData out_data;
	out_data.samples.resize(total_samples);

	for(uint32_t i= 0; i < total_samples; ++i)
	{
		const fixed16_t noise_coord= Fixed16MulDiv(IntToFixed16(int32_t(i)), int32_t(noise.size()), int32_t(total_samples));
		const size_t noise_coord_int= size_t(Fixed16FloorToInt(noise_coord));
		const fixed16_t noise_fract= noise_coord & (g_fixed16_one - 1);

		const int32_t value = Fixed16VecDot({noise[noise_coord_int], noise[noise_coord_int + 1]}, {g_fixed16_one - noise_fract, noise_fract});
		out_data.samples[i]= SampleType(value - 32768);
	}

	return out_data;
}

} // namespace Armed
