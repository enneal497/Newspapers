#include "DataManager.h"
#include "Utility.h"

namespace DataManager
{
	//Save set of used entry hashes
	bool SaveUsedEntries(SKSE::SerializationInterface* a_intfc)
	{
		logger::info("Saving used entries");
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
		logger::info("Saved used entries");
		return true;
	}

	//Save set of current entries and lastUpdatedDay
	bool SaveConfigData(SKSE::SerializationInterface* a_intfc)
	{
		logger::info("Saving current entries");
		const std::size_t mapSize = newspaperMap.size();
		if (!a_intfc->WriteRecordData(mapSize)) {
			logger::error("Failed to save map size");
			return false;
		}
		logger::info("Saved mapsize");
		for (const auto& [key, item] : newspaperMap) {
			//Save item
			if (!item.currentEntry) {
				logger::error("currentEntry not found");
				return false;
			}

			if (!a_intfc->WriteRecordData(key) ||
				!a_intfc->WriteRecordData(item.currentEntry->GetFormID())) {
				return false;
			}

			//Save book description
			std::size_t strSize = item.currentCNAM.length() + 1;
			logger::info("strSize: {}", strSize);
			if (!a_intfc->WriteRecordData(strSize) ||
				!a_intfc->WriteRecordData(item.currentCNAM.data(), static_cast<std::uint32_t>(strSize))) {
				return false;
			}

			//Save lastUpdatedDay
			if (!a_intfc->WriteRecordData(item.lastUpdatedDay)) {
				return false;
			}
		}
		logger::info("Saved current entries");
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
	bool LoadConfigData(SKSE::SerializationInterface* a_intfc)
	{
		std::size_t mapSize;
		a_intfc->ReadRecordData(&mapSize, sizeof(mapSize));
		logger::info("currentEntryMap size: {}", mapSize);

		if (mapSize == 0) { return true; }
		for (; mapSize > 0; --mapSize) {	
			bool bExistsInMap = true;
			//Load currentEntry
			std::string key;
			if (!a_intfc->ReadRecordData(&key, sizeof(key))) { return false; }
			logger::info("Key: {}", key);
			if (!newspaperMap.contains(key)) {
				//Skip remaining data;
				bExistsInMap = false;
			}

			RE::FormID a_bookID;
			if (!Utility::ReadFormID(a_intfc, a_bookID)) {
				logger::warn("Failed to resolve formID. Plugin may have been removed");
				return false;
			}
			logger::info("Read formID: {}", a_bookID);

			//Load book description
			std::string CNAM;
			std::size_t strSize = 0;
			if (!a_intfc->ReadRecordData(strSize)) { return false; }
			CNAM.reserve(strSize);
			if (!a_intfc->ReadRecordData(CNAM.data(), static_cast<std::uint32_t>(strSize))) { 
				return false; 
			}

			//Load lastUpdatedDay
			logger::info("Reading lastUpdatedDay");
			int32_t lastUpdatedDay;
			if (!a_intfc->ReadRecordData(&lastUpdatedDay, sizeof(lastUpdatedDay))) { return false; }
			logger::info("Read lastUpdatedDay: {}", lastUpdatedDay);

			//Refresh entries
			if (bExistsInMap) {
				auto* const boundOBJ = RE::TESForm::LookupByID<RE::TESBoundObject>(a_bookID);
				auto& newspaper = newspaperMap.at(key);
				newspaper.currentEntry = boundOBJ;
				newspaper.lastUpdatedDay = lastUpdatedDay;
				newspaper.PushNewEntry(a_bookID, false, CNAM.data());
			}
		}

		logger::info("Read currentEntries");
		return true;

	}

	//Iterate through newspapers and update if required
	void UpdateAllEntries(bool bForceUpdate)
	{
		const auto daysPassed = static_cast<int32_t>(RE::Calendar::GetSingleton()->GetDaysPassed());
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