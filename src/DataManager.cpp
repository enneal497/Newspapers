#include "DataManager.h"
#include "Utility.h"

namespace DataManager
{
	//Save set of used entry hashes
	bool SaveUsedEntries(SKSE::SerializationInterface* a_intfc)
	{
		const std::size_t setSize = usedEntrySet.size();
		if (!a_intfc->WriteRecordData(setSize)) {
			logger::error("Failed to save set size");
			return false;
		}
		for (const auto& item : usedEntrySet) {
			//Save item
			if (!a_intfc->WriteRecordData(item)) {
				logger::error("Failed to write formID {}", item);
				return false;
			}
		}
		return true;
	}

	//Save set of current entries
	bool SaveCurrentEntries(SKSE::SerializationInterface* a_intfc)
	{
		const std::size_t mapSize = newspaperMap.size();
		if (!a_intfc->WriteRecordData(mapSize)) {
			logger::error("Failed to save map size");
			return false;
		}
		for (const auto& [key, item] : newspaperMap) {
			//Save item
			//logger::info("CNAM size: {}", item.currentCNAM.size());
			//logger::info("CNAM value: {}", item.currentCNAM);
			if (!a_intfc->WriteRecordData(key) ||
				!a_intfc->WriteRecordData(item.currentEntry->GetFormID())) {
				return false;
			}

			std::size_t strSize = item.currentCNAM.length() + 1;
			if (!a_intfc->WriteRecordData(strSize) ||
				!a_intfc->WriteRecordData(item.currentCNAM.data(), static_cast<std::uint32_t>(strSize))) {
				return false;
			}
		}
		return true;
	}

	//Load set of used entry formIDs
	bool LoadUsedEntries(SKSE::SerializationInterface* a_intfc)
	{
		std::size_t setSize;
		a_intfc->ReadRecordData(&setSize, sizeof(setSize));
		usedEntrySet.clear();
		logger::info("usedEntrySet size: {}", setSize);
		
		if (setSize == 0) { return true; }
		for (; setSize > 0; --setSize) {
			RE::FormID a_bookID;
			if (!Utility::ReadFormID(a_intfc, a_bookID)) {
				logger::warn("Failed to resolve formID. Plugin may have been removed");
				continue;
			}
			usedEntrySet.insert(a_bookID);
		}
		return true;

	}

	//Update newspapers to their previous entries
	bool LoadCurrentEntries(SKSE::SerializationInterface* a_intfc)
	{
		std::size_t mapSize;
		a_intfc->ReadRecordData(&mapSize, sizeof(mapSize));
		logger::info("currentEntryMap size: {}", mapSize);

		if (mapSize == 0) { return true; }
		for (; mapSize > 0; --mapSize) {
			std::string key;
			if (!a_intfc->ReadRecordData(&key, sizeof(key))) { return false; }
			if (!newspaperMap.contains(key)) { continue; }

			logger::info("Read key: {}", key);

			RE::FormID a_bookID;
			if (!Utility::ReadFormID(a_intfc, a_bookID)) {
				logger::warn("Failed to resolve formID. Plugin may have been removed");
				return false;
			}

			logger::info("Read formID: {}", a_bookID);

			std::string CNAM;
			std::size_t strSize = 0;
			if (!a_intfc->ReadRecordData(strSize)) { return false; }
			logger::info("size: {}", strSize);
			CNAM.reserve(strSize);
			if (!a_intfc->ReadRecordData(CNAM.data(), static_cast<std::uint32_t>(strSize))) { 
				return false; 
			}
			logger::info("New CNAM: {}", CNAM.data());

			//Refresh entries
			newspaperMap.at(key).PushNewEntry(a_bookID, false, CNAM.data());
		}

		logger::info("Read currentEntries");
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
				if (!bForceUpdate) { newspaper.lastUpdatedDay = daysPassed; }
				newspaper.UpdateEntry();
			}
		}
	}

}