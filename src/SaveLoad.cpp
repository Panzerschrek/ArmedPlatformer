#include "SaveLoad.hpp"
#include "Log.hpp"
#include <cstdio>
#include <cstring>

namespace Armed
{

namespace
{

void FileRead(std::FILE* const file, void* buffer, const size_t size)
{
	size_t read_total= 0u;

	do
	{
		const auto read= std::fread(static_cast<char*>(buffer) + read_total, 1, size - read_total, file);
		if(read <= 0)
			break;

		read_total+= read;
	} while(read_total < size);
}

void FileWrite(std::FILE* const file, const void* buffer, const size_t size)
{
	size_t write_total= 0u;

	do
	{
		const auto write= std::fwrite(static_cast<const char*>(buffer) + write_total, 1, size - write_total, file);
		if(write <= 0)
			break;

		write_total+= write;;
	} while(write_total < size);
}

} // namespace

const char SaveHeader::c_expected_id[8]= "ArmedSv";

bool SaveData(const char* const file_name, const SaveLoadBuffer& data)
{
	std::FILE* const f= std::fopen(file_name, "wb");
	if( f == nullptr )
	{
		Log::Warning("Can not write save \"", file_name, "\"");
		return false;
	}

	SaveHeader header{};
	std::memcpy(header.id, SaveHeader::c_expected_id, sizeof(header.id));
	header.version= SaveHeader::c_expected_version;
	header.content_size= uint32_t(data.size());

	FileWrite(f, &header, sizeof(SaveHeader));
	FileWrite(f, data.data(), data.size());

	std::fclose(f);
	return true;
}

// Returns true, if all ok
bool LoadData( const char* const file_name, SaveLoadBuffer& out_data)
{
	FILE* const f= std::fopen(file_name, "rb");
	if(f == nullptr)
	{
		Log::Warning("Can not read save \"", file_name, "\".");
		return false;
	}

	std::fseek(f, 0, SEEK_END);
	const ssize_t file_size= std::ftell(f);
	std::fseek(f, 0, SEEK_SET);

	if(file_size < ssize_t(sizeof(SaveHeader)))
	{
		Log::Warning("Save file is broken - it is too small.");
		std::fclose(f);
		return false;
	}

	SaveHeader header{};
	FileRead(f, &header, sizeof(SaveHeader));

	if(std::memcmp(header.id, header.c_expected_id, sizeof(header.id) ) != 0)
	{
		Log::Warning( "Save file is not a PanzerChasm save." );
		std::fclose(f);
		return false;
	}
	if(header.version != SaveHeader::c_expected_version)
	{
		Log::Warning( "Save file has different version." );
		std::fclose(f);
		return false;
	}

	const size_t content_size= size_t(file_size) - sizeof(SaveHeader);
	if(header.content_size != content_size)
	{
		Log::Warning( "Save file has content size, different from actual file size." );
		std::fclose(f);
		return false;
	}

	out_data.resize(content_size);
	FileRead(f, out_data.data(), out_data.size());

	std::fclose(f);
	return true;
}

std::string GetSaveFileNameForSlot(const size_t slot_number)
{
	char name[256]{};
	std::snprintf(name, sizeof(name), "/save_%02zu.pcs", slot_number);
	return name;
}

} // namespace Armed
