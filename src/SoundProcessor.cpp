#include "SoundProcessor.hpp"

namespace Armed
{

SoundProcessor::SoundProcessor(SoundOut& sound_out)
	: sound_out_(sound_out)
{}

void SoundProcessor::MakeSound(const SoundId sound_id)
{
	sound_out_.LockChannels();

	for (Channel& channel : sound_out_.GetChannels())
	{
		if(channel.is_active)
			continue;

		channel.is_active= true;
		channel.position_samples= 0;
		channel.volume[0]= channel.volume[1]= g_fixed16_one;
		channel.src_sound_data= &sounds_[size_t(sound_id)];
		break;
	}

	sound_out_.UnlockChannels();
}

} // namespace Armed

