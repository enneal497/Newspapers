#pragma once
#include "Newspaper.h"

namespace DataManager {
	inline std::unordered_map<std::string, Newspaper> newspaperMap;

	bool SaveConfigData(SKSE::SerializationInterface* serialisation);
	bool LoadConfigData(SKSE::SerializationInterface* serialisation);
}