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
	for(uint32_t i= 0u; i < sample_count * g_left_and_right; ++i)
		mix_buffer_[i]= 0;

	// Perform some shift for volume to allow volume more than 1 and still use only 32bit operations.
	const int32_t c_volume_shift= 4;
	const int32_t c_volume_final_shift= g_fixed16_base - c_volume_shift;

	for(Channel& channel : channels_)
	{
		if(!(channel.is_active && channel.src_sound_data != nullptr))
			continue;

		const int32_t volume[2]{channel.volume[0] >> c_volume_shift, channel.volume[1] >> c_volume_shift};
		const uint32_t samples_to_fill= std::min(sample_count, uint32_t(channel.src_sound_data->samples.size()) - channel.position_samples);
		const SampleType* const src= channel.src_sound_data->samples.data() + channel.position_samples;
		for(uint32_t i= 0; i < samples_to_fill; ++i)
		for(uint32_t side= 0; side < g_left_and_right; ++side)
		{
			const int32_t value= (int32_t(src[i]) * volume[side]) >> c_volume_final_shift;
			mix_buffer_[i * g_left_and_right + side]+= value;
		}

		channel.position_samples+= samples_to_fill;
		channel.is_active= channel.position_samples < uint32_t(channel.src_sound_data->samples.size());
	} // for channels

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
