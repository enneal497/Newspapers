#pragma once
#include "Newspaper.h"

namespace DataManager {
	inline std::unordered_map<std::string, Newspaper> newspaperMap;
	inline std::unordered_set<RE::FormID> usedEntrySet;

	const std::string_view dir{ R"(Data\SKSE\Plugins\Newspapers)" };
	const std::filesystem::path configDir{ R"(Data\SKSE\Plugins\Newspapers\Config)" };

	bool SaveUsedEntries(SKSE::SerializationInterface* a_intfc);
	bool LoadUsedEntries(SKSE::SerializationInterface* a_intfc);
	bool SaveConfigData(SKSE::SerializationInterface* a_intfc);
	bool LoadConfigData(SKSE::SerializationInterface* a_intfc);

	void UpdateAllEntries(bool bForceUpdate = false);
}