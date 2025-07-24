#pragma once
#include "Newspaper.h"

namespace DataManager {
	inline std::unordered_map<std::string, Newspaper> newspaperMap;

	bool SaveConfigData(SKSE::SerializationInterface* a_intfc);
	bool LoadConfigData(SKSE::SerializationInterface* a_intfc);
}