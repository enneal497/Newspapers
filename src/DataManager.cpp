#include "DataManager.h"
#include "Utility.h"

Newspaper::Newspaper(const LookupConfigs::configFormat &configData) {
	bookOLD = configData.bookName1;
	bookNEW = configData.bookName2;

	//Get formlist pointers
	for (auto formStr : configData.formlists) {
		auto formlist = Utility::GetFormFromID<RE::BGSListForm>(formStr);
		if (formlist) {
			formlists.push_back(formlist);
		}
		else {
			logger::warn("Skipping {} - formlist not found", formStr);
		}
	}

	//Get container pointers
	for (auto formStr : configData.containers) {
		auto container = Utility::GetFormFromID<RE::TESContainer>(formStr);
		if (container) {
			containers.push_back(container);
			containerIDs.push_back(formStr);
		}
		else {
			logger::warn("Skipping {} - container not found", formStr);
		}
	}

}

bool DataManager::SaveConfigData(SKSE::SerializationInterface* serialisation)
{
	const std::size_t mapSize = newspaperMap.size();
	if (!serialisation->WriteRecordData(mapSize)) {
		logger::error("Failed to save map size");
		return false;
	}
	for (auto& [key, value] : newspaperMap) {
		//Save key
		if (!serialisation->WriteRecordData(key)) {
			logger::error("Failed to write key {}", key);
			return false;
		}
		//Save bookNames
		if (!serialisation->WriteRecordData(value.GetBookOLD()) ||
			!serialisation->WriteRecordData(value.GetBookOLD())) {
			return false;
		}
		//Save formlists
		const auto formlists = value.GetFormlists();
		if (!serialisation->WriteRecordData(formlists.size())) { return false; }
		for (const auto formlist : formlists) {
			if (!serialisation->WriteRecordData(formlist->GetFormID())) { 
				return false; 
			}
		}
		//Save containers
		const auto containerIDs = value.GetContainerIDs();
		if (!serialisation->WriteRecordData(containerIDs.size())) { return false; }
		for (const auto containerID : containerIDs) {
			if (!serialisation->WriteRecordData(containerID)) {
				return false;
			}
		}
	}
	return true;
}

bool DataManager::LoadConfigData(SKSE::SerializationInterface* serialisation)
{
	std::size_t mapSize;
	serialisation->ReadRecordData(&mapSize, sizeof(mapSize));
	return true;
}