#pragma once
#include "Fixed.hpp"
#include <array>
#include <memory>
#include <vector>
#include <SDL_audio.h>

namespace Armed
{

using SampleType= int16_t;

struct SoundData
{
	std::vector<SampleType> samples;
};

struct Channel
{
	static constexpr uint32_t c_max_channels= 32u;
	static constexpr uint32_t  c_left_channel_number= 0u;
	static constexpr uint32_t c_right_channel_number= 1u;

	bool is_active= false;
	fixed16_t volume= 0;

	uint32_t position_samples= 0;

	SoundData* src_sound_data= nullptr;
};

using Channels= std::array<Channel, Channel::c_max_channels>;

class SoundOut final
{
public:
	SoundOut();
	~SoundOut();

	void LockChannels();
	void UnlockChannels();
	Channels& GetChannels(){ return channels_; }

	uint32_t GetSampleRate() const { return sample_rate_; }

private:
	static void SDLCALL AudioCallback(void* userdata, Uint8* stream, int len_bytes);
	void FillAudioBuffer(SampleType* buffer, uint32_t sample_count);

private:
	Channels channels_;

	SDL_AudioDeviceID device_id_= 0u;
	uint32_t sample_rate_= 0u; // samples per second

	std::vector<int32_t> mix_buffer_;
};

} // namespace Armed
