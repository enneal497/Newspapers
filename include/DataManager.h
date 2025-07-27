#pragma once
#include "Newspaper.h"

namespace DataManager {
	inline std::unordered_map<std::string, Newspaper> newspaperMap;
	inline std::unordered_set<uint32_t> usedEntrySet;

	bool SaveConfigData(SKSE::SerializationInterface* a_intfc);
	bool LoadConfigData(SKSE::SerializationInterface* a_intfc);
	bool SaveEntryData(SKSE::SerializationInterface* a_intfc);
	bool LoadEntryData(SKSE::SerializationInterface* a_intfc);
}