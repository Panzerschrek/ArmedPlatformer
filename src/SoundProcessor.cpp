#include "SoundProcessor.hpp"
#include "SoundsGeneration.hpp"

namespace Armed
{

SoundProcessor::SoundProcessor(SoundOut& sound_out)
	: sound_out_(sound_out)
{
	for(size_t i= 0; i < size_t(SoundId::NumSounds); ++i)
	{
		sounds_[i]= GetSinWaveSound(sound_out_.GetFrequency(), IntToFixed16(440), 440);
	}
}

void SoundProcessor::MakeSound(const SoundId sound_id)
{
	sound_out_.LockChannels();

	for (Channel& channel : sound_out_.GetChannels())
	{
		if(channel.is_active)
			continue;

		channel.is_active= true;
		channel.position_samples= 0;
		channel.volume[0]= channel.volume[1]= g_fixed16_one / 3;
		channel.src_sound_data= &sounds_[size_t(sound_id)];
		break;
	}

	sound_out_.UnlockChannels();
}

} // namespace Armed

