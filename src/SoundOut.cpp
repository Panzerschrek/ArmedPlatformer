#include "SoundOut.hpp"
#include "Assert.hpp"
#include "Log.hpp"
#include <SDL.h>
#include <limits>

namespace Armed
{

namespace
{

const SDL_AudioDeviceID g_first_valid_device_id= 2u;
const uint32_t g_left_and_right= 2u;

int32_t NearestPowerOfTwoFloor(const int32_t x)
{
	int32_t r= 1 << 30;
	while(r > x) r>>= 1;
	return r;
}

} // namespace

SoundOut::SoundOut()
{
	SDL_InitSubSystem(SDL_INIT_AUDIO);

	SDL_AudioSpec requested_format;
	SDL_AudioSpec obtained_format;

	requested_format.channels= g_left_and_right;
	requested_format.freq= 22050;
	requested_format.format= AUDIO_S16;
	requested_format.callback= AudioCallback;
	requested_format.userdata= this;

	// ~ 1 callback call per two frames (60fps)
	requested_format.samples= Uint16(NearestPowerOfTwoFloor(requested_format.freq / 30));

	int device_count= SDL_GetNumAudioDevices(0);
	// Can't get explicit devices list. Trying to use first device.
	if(device_count == -1)
		device_count= 1;

	for(int i= 0; i < device_count; i++)
	{
		const char* const device_name= SDL_GetAudioDeviceName(i, 0);

		const SDL_AudioDeviceID device_id=
			SDL_OpenAudioDevice(device_name, 0, &requested_format, &obtained_format, 0);

		if(device_id >= g_first_valid_device_id &&
			obtained_format.channels == requested_format.channels &&
			obtained_format.format   == requested_format.format)
		{
			device_id_= device_id;
			Log::Info("Open audio device: ", device_name);
			break;
		}
	}

	if(device_id_ < g_first_valid_device_id)
	{
		Log::FatalError("Can not open any audio device");
		return;
	}

	frequency_= uint32_t(obtained_format.freq);

	mix_buffer_.resize(obtained_format.samples * g_left_and_right);

	// Run
	SDL_PauseAudioDevice(device_id_ , 0);
}

SoundOut::~SoundOut()
{
	if(device_id_ >= g_first_valid_device_id)
		SDL_CloseAudioDevice(device_id_);

	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void SoundOut::LockChannels()
{
	if(device_id_ >= g_first_valid_device_id)
		SDL_LockAudioDevice(device_id_);
}

void SoundOut::UnlockChannels()
{
	if(device_id_ >= g_first_valid_device_id)
		SDL_UnlockAudioDevice(device_id_);
}

void SDLCALL SoundOut::AudioCallback(void* const userdata, Uint8* const stream, int len_bytes)
{
	const auto self= reinterpret_cast<SoundOut*>(userdata);

	self->FillAudioBuffer(
		reinterpret_cast<SampleType*>(stream),
		uint32_t(len_bytes) / (sizeof(SampleType) * g_left_and_right));
}

void SoundOut::FillAudioBuffer(SampleType* const buffer, const uint32_t sample_count)
{
	ARMED_ASSERT(sample_count * g_left_and_right <= mix_buffer_.size());

	// Zero mix buffer.
	for(uint32_t i= 0u; i < sample_count * g_left_and_right; i++)
		mix_buffer_[i]= 0;

	/*
	for(Channel& channel : channels_)
	{
		if(!(channel.is_active && channel.src_sound_data != nullptr))
			continue;

		uint32_t channel_dst_samples_processed= 0u;
		do
		{
			const auto max_dst_sample_for_src_sample=
			[&](const uint32_t i) -> uint32_t
			{
				if(i == 0u) return 0u;
				const uint32_t result= ((i << g_frac_bits) - 1u) / freq_ratio_f;
				ARMED_ASSERT(((result * freq_ratio_f) >> g_frac_bits) < i);
				return result + 1u;
			};

			int32_t* const dst= mix_buffer_.data() + channel_dst_samples_processed * g_left_and_right;
			const uint32_t can_read_src_samples= std::max(int32_t(channel.src_sound_data->sample_count_ - channel.position_samples) - 1, 0);
			const uint32_t dst_samples_to_write=
				std::min(
					sample_count - channel_dst_samples_processed,
					max_dst_sample_for_src_sample(can_read_src_samples));


			channel.position_samples+= ((sample_count - channel_dst_samples_processed) * freq_ratio_f) >> g_frac_bits;
			channel.position_samples= std::min(channel.position_samples, channel.src_sound_data->sample_count_);
			channel_dst_samples_processed+= dst_samples_to_write;

			} while(channel_dst_samples_processed < sample_count);
	} // for channels
	*/

	// Copy mix buffer to result buffer.
	for(uint32_t i= 0u; i < sample_count * g_left_and_right; i++)
		buffer[i]=
			SampleType(
				std::max(
					int32_t(std::numeric_limits<SampleType>::min()),
					std::min(
						mix_buffer_[i],
						int32_t(std::numeric_limits<SampleType>::max()))));
}

} // namespace Armed
