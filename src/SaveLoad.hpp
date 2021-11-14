#pragma once
#include "Assert.hpp"
#include <string>
#include <vector>

namespace Armed
{

using SaveLoadBuffer= std::vector<uint8_t>;

struct SaveHeader
{
public:
	using HashType= uint32_t;

	static const char c_expected_id[8];
	static constexpr uint32_t c_expected_version= 0x100u; // Change each time, when format changed.

public:
	char id[8]; // must be equal to c_expected_id
	uint32_t version;
	uint32_t content_size;
};

// Returns true, if all ok
bool SaveData(const char* file_name, const SaveLoadBuffer& data);

// Returns true, if all ok
bool LoadData(const char* file_name, SaveLoadBuffer& out_data );

std::string GetSaveFileNameForSlot(size_t slot_number);

void CreateSlotSavesDir();

} // namespace Armed
