#include "DataManager.h"
#include "LookupConfigs.h"
#include "Utility.h"

namespace DataManager
{
	//Save newspaper config data
	bool SaveConfigData(SKSE::SerializationInterface* a_intfc)
	{
		const std::size_t mapSize = newspaperMap.size();
		if (!a_intfc->WriteRecordData(mapSize)) {
			logger::error("Failed to save map size");
			return false;
		}
		for (auto& [key, value] : newspaperMap) {
			//Save key
			if (!a_intfc->WriteRecordData(key)) {
				logger::error("Failed to write key {}", key);
				return false;
			}
			if (!a_intfc->WriteRecordData(value.updateInterval)) {
				return false;
			}
			//Save bookNames
			if (!a_intfc->WriteRecordData(value.GetBook()->GetFormID())) {
				return false;
			}
		}
		return true;
	}

	//Load all newspaper config data
	bool LoadConfigData(SKSE::SerializationInterface* a_intfc)
	{
		std::size_t mapSize;
		a_intfc->ReadRecordData(&mapSize, sizeof(mapSize));
		newspaperMap.clear();

		logger::info("mapSize: {}", mapSize);

		if (mapSize == 0) { return false; }

		for (; mapSize > 0; --mapSize) {
			std::string key;
			float updateInterval;
			RE::FormID bookID;

			a_intfc->ReadRecordData(&key, sizeof(key));
			a_intfc->ReadRecordData(&updateInterval, sizeof(updateInterval));
			a_intfc->ReadRecordData(&bookID, sizeof(bookID));

			//Resolve formIDs
			if (!a_intfc->ResolveFormID(bookID, bookID)) {
				logger::warn("Failed to resolve formID {}", bookID);
				continue;
			}

			auto bookOBJ = RE::TESForm::LookupByID<RE::TESObjectBOOK>(bookID);
			if (!bookOBJ) { logger::warn("Failed to find book form"); continue; }

			auto result = newspaperMap.try_emplace(key, bookOBJ, updateInterval);
			if (!result.second) {
				logger::critical("Failed to read key");
				return false;
			}

		}

		//Read and append formIDs from file
		LookupConfigs::ReadConfigsFromFile(false);

		return true;
	}

	//Save set of used entry hashes
	bool SaveEntryData(SKSE::SerializationInterface* a_intfc)
	{
		const std::size_t setSize = usedEntrySet.size();
		if (!a_intfc->WriteRecordData(setSize)) {
			logger::error("Failed to save set size");
			return false;
		}
		for (const auto& item : usedEntrySet) {
			//Save item
			if (!a_intfc->WriteRecordData(item)) {
				logger::error("Failed to write hash {}", item);
				return false;
			}
		}
		return true;
	}

	//Load set of used entry hashes
	bool LoadEntryData(SKSE::SerializationInterface* a_intfc)
	{
		std::size_t setSize;
		a_intfc->ReadRecordData(&setSize, sizeof(setSize));
		usedEntrySet.clear();

		logger::info("Set size: {}", setSize);

		if (setSize == 0) { return true; }

		for (; setSize > 0; --setSize) {
			uint32_t item;
			a_intfc->ReadRecordData(&item, sizeof(item));
			usedEntrySet.insert(item);
		}
		return true;

	}

	//Iterate through newspapers and update if required
	void UpdateAllEntries(bool bForceUpdate)
	{
		const auto daysPassed = RE::Calendar::GetSingleton()->GetDaysPassed();
		logger::info("daysPassed: {}", daysPassed);

		for (auto& [key, newspaper] : newspaperMap) {
			if (bForceUpdate || newspaper.lastUpdatedDay + newspaper.updateInterval < daysPassed) {
				//Update newspaper entries
				logger::info("Updating entry for {}", key);
				newspaper.lastUpdatedDay = daysPassed;
				newspaper.UpdateEntry();
			}
		}
	}

}