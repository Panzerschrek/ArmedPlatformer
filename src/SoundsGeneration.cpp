#include "SoundsGeneration.hpp"
#include "Rand.hpp"
#include <limits>

namespace Armed
{

namespace
{

const int32_t g_samle_scale= int32_t(std::numeric_limits<SampleType>::max());
const float g_two_pi = 2.0f * 3.1415926535f;

SampleType ClampSample(const int32_t x)
{
	return
		SampleType(
			std::max(
				int32_t(std::numeric_limits<SampleType>::min()),
				std::min(
					x,
					int32_t(std::numeric_limits<SampleType>::max()))));
}

std::vector<int32_t> LinearResample(const std::vector<int32_t>& data, const size_t new_size)
{
	std::vector<int32_t> res;
	res.resize(new_size);
	const int32_t c_shift= 8;
	const int32_t c_scale= 1 << c_shift;
	for(size_t i= 0; i < res.size(); ++i)
	{
		const int64_t coord= ((int64_t(i) * int64_t(data.size())) << c_shift) / int64_t(res.size());
		const size_t coord_int= size_t(coord >> c_shift);
		const size_t coord_int_plus_one= std::min(res.size() - 1, coord_int + 1);
		const int32_t noise_fract= coord & (c_scale - 1);
		const int32_t value= int32_t((int64_t(data[coord_int]) * (int64_t(c_scale) - noise_fract) + int64_t(data[coord_int_plus_one] * noise_fract)) >> c_shift);
		res[i]= value;
	}

	return res;
}

std::vector<int32_t> GenRandomNoise(const size_t size)
{
	std::vector<int32_t> noise;
	noise.resize(size);

	Rand rand;
	for(size_t i= 0; i < noise.size(); ++i)
		noise[i]= int32_t(rand.RandValue(0, 65536)) - 32768;

	return noise;
}

std::vector<int32_t> GenOctaveNoise(const size_t size, const uint32_t octaves)
{
	std::vector<int32_t> res;
	res.resize(size, 0);
	for(uint32_t octave= 0; octave < octaves; ++octave)
	{
		const std::vector<int32_t> noise= LinearResample(GenRandomNoise(size >> octave), size);
		for(size_t i= 0; i < res.size(); ++i)
			res[i]+= noise[i] >> (octaves - octave);
	}

	return res;
}

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

SoundData GenShotSound(const uint32_t frequency)
{
	const uint32_t total_samples= frequency;
	SoundData out_data;
	out_data.samples.resize(total_samples);

	const std::vector<int32_t> noise= LinearResample(GenOctaveNoise(8192, 5), total_samples);

	const float fade_factor= -16.0f / float(frequency);
	const float hyp_factor= 2048.0f / float(frequency);
	const float constant_scale= 4.0f;
	for(uint32_t i= 0; i < total_samples; ++i)
	{
		const float hyp_scale= 1.0f - 1.0f / (float(i) * hyp_factor + 1.0f);
		const float exp_scale= std::exp(float(i) * fade_factor);
		out_data.samples[i]= ClampSample(int32_t(float(noise[i]) * hyp_scale * exp_scale * constant_scale));
	}

	return out_data;
}

SoundData GenExplosionSound(const uint32_t frequency)
{
	const uint32_t total_samples= 3 * frequency;
	SoundData out_data;
	out_data.samples.resize(total_samples);

	const std::vector<int32_t> noise= LinearResample(GenOctaveNoise(8192, 4), total_samples);

	const float fade_factor= -3.0f / float(frequency);
	const float hyp_factor= 512.0f / float(frequency);
	const float constant_scale= 8.0f;
	for(uint32_t i= 0; i < total_samples; ++i)
	{
		const float hyp_scale= 1.0f - 1.0f / (float(i) * hyp_factor + 1.0f);
		const float exp_scale= std::exp(float(i) * fade_factor);
		out_data.samples[i]= ClampSample(int32_t(float(noise[i]) * hyp_scale * exp_scale * constant_scale));
	}

	return out_data;
}

} // namespace Armed
