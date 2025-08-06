#pragma once
#include "Newspaper.h"

namespace DataManager {
	inline std::unordered_map<std::string, Newspaper> newspaperMap;
	inline std::unordered_set<RE::FormID> usedEntrySet;

	const std::string_view dir{ R"(Data\SKSE\Newspapers\)" };
	const std::filesystem::path configDir{ R"(Data\SKSE\Newspapers\Config)" };

	bool SaveEntryData(SKSE::SerializationInterface* a_intfc);
	bool LoadEntryData(SKSE::SerializationInterface* a_intfc);

	void UpdateAllEntries(bool bForceUpdate = false);
}