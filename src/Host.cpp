#include "Assert.hpp"
#include "Host.hpp"
#include "Maps/Maps.hpp"
#include <thread>

namespace Armed
{

Host::CurrentWolrdData::CurrentWolrdData(
	const MapDescription& map_description,
	SystemWindow& system_window,
	SoundProcessor& sound_processor)
	: world(map_description, sound_processor)
	, camera(world, system_window)
	, world_view(world, camera, system_window)
	, hud(world, system_window)
{
}

Host::CurrentWolrdData::CurrentWolrdData(World in_world, SystemWindow& system_window)
	: world(std::move(in_world))
	, camera(world, system_window)
	, world_view(world, camera, system_window)
	, hud(world, system_window)
{
}

Host::Host()
	: system_window_()
	, sound_out_()
	, sound_processor_(sound_out_)
	, menu_(
		system_window_,
		{
			[this]{NewGame();},
			[this](const size_t slot){SaveGame(slot);},
			[this](const size_t slot){LoadGame(slot);},
			[this]{Quit();},
			[this]{return SaveAvailable();},
			[this](const size_t slot){return HasSave(slot);},
		})
	, init_time_(Clock::now())
	, prev_tick_time_(GetCurrentTime())
{
}

bool Host::Loop()
{
	const auto tick_start_time= GetCurrentTime();

	const SystemEvents system_events= system_window_.ProcessEvents();
	for(const SystemEvent& system_event : system_events)
	{
		if(std::get_if<SystemEventTypes::QuitEvent>(&system_event) != nullptr)
			return true;

		if(const auto key_event= std::get_if<SystemEventTypes::KeyEvent>(&system_event))
		{
			if(key_event->pressed &&
				key_event->key_code >= SystemEventTypes::KeyCode::F1 &&
				size_t(key_event->key_code) <= size_t(SystemEventTypes::KeyCode::F1) + std::size(Maps::maps_list) - 1)
			{
				NewGame(size_t(key_event->key_code) - size_t(SystemEventTypes::KeyCode::F1));
			}
		}
	}

	menu_.ProcessInput(system_events);

	// Begin frame to make window surface available for view matrix calculation.
	system_window_.BeginFrame();

	if(current_world_data_ != std::nullopt && !menu_.IsActive())
	{
		// Perform some ticks. Possible 0, 1 or many. But do not perform more than 5 ticks once.
		for (
			uint64_t
				physics_start_tick = prev_tick_time_ * World::c_update_frequency / c_time_point_resolution,
				physics_end_tick = tick_start_time * World::c_update_frequency / c_time_point_resolution,
				t= physics_start_tick,
				iterations= 0;
			t < physics_end_tick && iterations < 5;
			++t, ++iterations)
		{
			const InputState input_state= system_window_.GetInputState();
			current_world_data_->world.Tick(input_state, current_world_data_->camera.GetAimVec());
			current_world_data_->camera.Update(input_state);

			if(current_world_data_->world.ShouldChageMap())
			{
				sound_processor_.StopAllSounds();
				current_map_= (current_map_ + 1) % std::size(Maps::maps_list);
				current_world_data_.emplace(Maps::maps_list[current_map_], system_window_, sound_processor_);
				break;
			}
		}
	}

	if(current_world_data_ != std::nullopt)
	{
		current_world_data_->world_view.Draw();
		current_world_data_->hud.Draw();
	}
	menu_.Draw();
	system_window_.EndFrame();

	const TimePoint tick_end_time= GetCurrentTime();
	const auto frame_dt= tick_end_time - tick_start_time;

	const uint64_t max_fps= 120;
	const auto min_frame_duration = c_time_point_resolution / max_fps;
	if(frame_dt <= min_frame_duration)
	{
		std::this_thread::sleep_for(ChronoDuration(min_frame_duration - frame_dt));
	}

	prev_tick_time_= tick_start_time;

	return quit_requested_;
}

Host::TimePoint Host::GetCurrentTime()
{
	const Clock::time_point now= Clock::now();
	const auto dt= now - init_time_;

	return TimePoint(std::chrono::duration_cast<ChronoDuration>(dt).count());
}

void Host::NewGame(const size_t map_index)
{
	sound_processor_.StopAllSounds();
	current_world_data_.emplace(Maps::maps_list[map_index], system_window_, sound_processor_);
}

void Host::SaveGame(const size_t slot)
{
	if(current_world_data_ == std::nullopt)
		return;

	SaveLoadBuffer buffer;
	{
		SaveStream stream(buffer);
		stream.Write(current_map_);
		current_world_data_->world.Save(stream);
	}

	SaveData(GetSaveFileNameForSlot(slot).c_str(), buffer);
}

void Host::LoadGame(const size_t slot)
{
	sound_processor_.StopAllSounds();

	SaveLoadBuffer buffer;
	if (!LoadData(GetSaveFileNameForSlot(slot).c_str(), buffer))
		return;

	LoadStream stream(buffer, 0);
	stream.Read(current_map_);

	current_world_data_= std::nullopt;
	current_world_data_.emplace(World::Load(sound_processor_, stream), system_window_);
}

void Host::Quit()
{
	quit_requested_= true;
}

bool Host::SaveAvailable()
{
	return current_world_data_ != std::nullopt && current_world_data_->world.GetPlayer().GetHealth() > 0;
}

bool Host::HasSave(const size_t slot)
{
	FILE* const f= std::fopen(GetSaveFileNameForSlot(slot).c_str(), "rb");
	if(f != nullptr)
	{
		std::fclose(f);
		return true;
	}
	return false;
}

} // namespace Armed
