#pragma once
#include "LookupConfigs.h"

class Newspaper 
{
private:
	std::string bookOLD;
	std::string bookNEW;
	std::vector<RE::BGSListForm*> formlists;
	std::vector<RE::TESContainer*> containers;

	std::vector<std::string> containerIDs;
public:
	Newspaper(const LookupConfigs::configFormat &configData);

	std::string GetBookOLD() { return bookOLD; }
	std::string GetBookNEW() { return bookNEW; }
	std::vector<RE::BGSListForm*> GetFormlists() { return formlists; }
	std::vector < RE::TESContainer*> GetContainers() { return containers; }
	std::vector<std::string> GetContainerIDs() { return containerIDs; }
};

namespace DataManager {
	inline std::unordered_map<std::string, Newspaper> newspaperMap;

	bool SaveConfigData(SKSE::SerializationInterface* serialisation);
	bool LoadConfigData(SKSE::SerializationInterface* serialisation);
}