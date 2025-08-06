#pragma once
#include "Newspaper.h"

namespace DataManager {
	inline std::unordered_map<std::string, Newspaper> newspaperMap;
	inline std::unordered_set<RE::FormID> usedEntrySet;

	const auto dir{ R"(Data\SKSE\Newspapers\)" };

	bool SaveEntryData(SKSE::SerializationInterface* a_intfc);
	bool LoadEntryData(SKSE::SerializationInterface* a_intfc);

	void UpdateAllEntries(bool bForceUpdate = false);
}