#include "SoundsGeneration.hpp"
#include "Assert.hpp"
#include "Rand.hpp"
#include <limits>

namespace Armed
{

namespace
{

const int32_t g_sample_scale= int32_t(std::numeric_limits<SampleType>::max());
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
		ARMED_ASSERT(coord_int < data.size());
		const size_t coord_int_plus_one= std::min(data.size() - 1, coord_int + 1);
		const int64_t noise_fract= coord & (c_scale - 1);
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

SoundData GenSinWaveSound(const uint32_t sample_rate, const fixed16_t sin_wave_frequency, const uint32_t periods)
{
	const uint32_t total_samples= uint32_t(uint64_t(sample_rate) * uint64_t(periods) * uint64_t(g_fixed16_one) / uint64_t(sin_wave_frequency));

	SoundData out_data;
	out_data.samples.resize(total_samples);

	const float scale_f= float(sin_wave_frequency) * g_two_pi / (float(sample_rate) * float(g_fixed16_one));
	const float sample_scale= float(g_sample_scale);
	for(uint32_t i= 0; i < total_samples; ++i)
		out_data.samples[i]= SampleType(std::sin(float(i) * scale_f) * sample_scale);

	return out_data;
}

SoundData GenExplosionSound(const uint32_t sample_rate)
{
	const uint32_t total_samples= 3 * sample_rate;
	SoundData out_data;
	out_data.samples.resize(total_samples);

	const std::vector<int32_t> noise= LinearResample(GenOctaveNoise(8192, 4), total_samples);

	const float fade_factor= -3.0f / float(sample_rate);
	const float hyp_factor= 512.0f / float(sample_rate);
	const float constant_scale= 8.0f;
	for(uint32_t i= 0; i < total_samples; ++i)
	{
		const float hyp_scale= 1.0f - 1.0f / (float(i) * hyp_factor + 1.0f);
		const float exp_scale= std::exp(float(i) * fade_factor);
		out_data.samples[i]= ClampSample(int32_t(float(noise[i]) * hyp_scale * exp_scale * constant_scale));
	}

	return out_data;
}

SoundData GenShotSound(const uint32_t sample_rate)
{
	const uint32_t total_samples= sample_rate;
	SoundData out_data;
	out_data.samples.resize(total_samples);

	const std::vector<int32_t> noise= LinearResample(GenOctaveNoise(8192, 5), total_samples);

	const float fade_factor= -16.0f / float(sample_rate);
	const float hyp_factor= 2048.0f / float(sample_rate);
	const float constant_scale= 4.0f;
	for(uint32_t i= 0; i < total_samples; ++i)
	{
		const float hyp_scale= 1.0f - 1.0f / (float(i) * hyp_factor + 1.0f);
		const float exp_scale= std::exp(float(i) * fade_factor);
		out_data.samples[i]= ClampSample(int32_t(float(noise[i]) * hyp_scale * exp_scale * constant_scale));
	}

	return out_data;
}

SoundData GenPickUpSound(const uint32_t sample_rate)
{
	const uint32_t total_samples= sample_rate;
	SoundData out_data;
	out_data.samples.resize(total_samples);

	const float base_freq= 400.0f;
	const float scale_f= float(base_freq) * g_two_pi / float(sample_rate);
	const float sample_scale= float(g_sample_scale);

	for(uint32_t i= 0; i < total_samples; ++i)
	{
		float a[4];
		a[3]= float(total_samples - i) / float(total_samples);
		a[2]= a[3] * a[3];
		a[1]= a[2] * a[2];
		a[0]= a[2] * a[1];

		const float t= float(i) * scale_f;

		float s=
			 0.5f * a[0] * std::sin(t) +
			0.25f * a[1] * std::sin(t * 2.0f) +
			0.25f * a[2] * std::sin(t * 3.0f) +
			0.25f * a[3] * std::sin(t * 4.0f);

		out_data.samples[i]= SampleType(s * sample_scale);
	}

	return out_data;
}

SoundData GenBiteSound(const uint32_t sample_rate)
{
	const uint32_t total_samples= sample_rate / 3;
	SoundData out_data;
	out_data.samples.resize(total_samples);

	std::vector<int32_t> noise= LinearResample(GenOctaveNoise(128, 3), total_samples / 4);
	for(int32_t& val : noise)
		val= val > 0 ? (+g_sample_scale) : (-g_sample_scale);
	std::vector<int32_t> noise_resampled= LinearResample(noise, total_samples);

	const float fade_factor= -24.0f / float(sample_rate);
	const float hyp_factor= 128.0f / float(sample_rate);
	const float constant_scale= 4.0f;
	for(uint32_t i= 0; i < total_samples; ++i)
	{
		const float hyp_scale= 1.0f - 1.0f / (float(i) * hyp_factor + 1.0f);
		const float exp_scale= std::exp(float(i) * fade_factor);

		out_data.samples[i]= ClampSample(int32_t(float(noise_resampled[i]) * hyp_scale * exp_scale * constant_scale));
	}

	return out_data;
}

SoundData GenPlayerSpawn(const uint32_t sample_rate)
{
	const uint32_t total_samples= sample_rate / 2;
	SoundData out_data;
	out_data.samples.resize(total_samples);

	const float c_base_freq= 40.0f;
	const float c_pulsations_freq_divider= 20.0f;
	const float c_exp_factor= 1.5f;
	const float c_power_factor= 1.0f / 3.0f;

	for(uint32_t i= 0; i < total_samples; ++i)
	{
		const float t= float(i) / float(total_samples);
		const float freq= c_base_freq * std::exp(t * c_exp_factor);

		const float p= g_two_pi * t;
		const float a= 1.0f - (std::cos(freq / c_pulsations_freq_divider * p) * 0.5f + 0.5f) * std::pow(t, c_power_factor);
		const float s= std::sin(p * freq);
		out_data.samples[i]= ClampSample(int32_t(a * s * float(g_sample_scale)));
	}

	return out_data;
}

SoundData GenMapEndMelody(const uint32_t sample_rate)
{
	const uint32_t c_samples_per_base_wave= 1 << 7;
	const uint32_t c_beat_waves= 64;
	const uint32_t c_beat_period= c_samples_per_base_wave * c_beat_waves;
	const uint32_t c_start_beat= 2;
	const uint32_t c_end_beat= 9;
	const uint32_t c_base_period_waves= c_beat_period * c_end_beat * 3;

	const uint32_t samples_initial= c_base_period_waves * 8;

	std::vector<int32_t> data;
	data.resize(samples_initial);

	const float sample_scale= float(g_sample_scale) * 0.5f / float(1 + c_end_beat - c_start_beat);

	for(uint32_t i= 0; i < samples_initial; ++i)
	{
		const uint32_t i_masked= i & (c_samples_per_base_wave - 1);
		const float val= float(i_masked) / float(c_samples_per_base_wave);
		const float t= float(val) * g_two_pi;
		const float base_wave= std::sin(t) + std::sin(t * 2.0f) * 0.5f;

		float result= base_wave;

		// See https://www.youtube.com/watch?v=Qyn64b4LNJ0.
		for(uint32_t beat_n= c_start_beat; beat_n <= c_end_beat; ++beat_n)
		{
			const uint32_t beat_factor= i % (c_beat_period * beat_n);
			if(beat_factor < c_beat_period * beat_n / 8)
			{
				const float beat_t= t * float(beat_n);
				result+= std::sin(beat_t) + std::sin(beat_t * 2.0f) * 0.5f + std::sin(beat_t * 3.0f) * 0.3333f;
			}
		}

		data[i]= int32_t(result * sample_scale);
	}

	const uint32_t c_base_freq= 220;
	const uint32_t samples_final= uint32_t(uint64_t(samples_initial) * uint64_t(sample_rate) / (uint64_t(c_base_freq) * uint64_t(c_samples_per_base_wave)));

	const std::vector<int32_t> data_resampled= LinearResample(data, samples_final);

	SoundData out_data;
	out_data.samples.resize(data_resampled.size());
	for(size_t i= 0; i < data_resampled.size(); ++i)
		out_data.samples[i]= ClampSample(data_resampled[i]);

	return out_data;
}

} // namespace Armed
