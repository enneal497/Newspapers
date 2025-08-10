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
			return false;
		}
		std::vector<RE::FormID> ids(usedEntrySet.begin(), usedEntrySet.end());
		if (!a_intfc->WriteRecordData(ids)) {
			return false;
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
			return false;
		}

		for (const auto& [key, item] : newspaperMap) {
			//Save item
			if (!item.currentEntry) {
				logger::error("currentEntry not found");
				return false;
			}

			std::size_t keySize = key.size();
			if (!a_intfc->WriteRecordData(keySize) ||
				!a_intfc->WriteRecordData(key.data(), static_cast<std::uint32_t>(keySize)) ||
				!a_intfc->WriteRecordData(item.currentEntry->GetFormID())) {
				return false;
			}

			//Save book description
			std::size_t strSize = item.currentCNAM.size();
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
		usedEntrySet.reserve(setSize);
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
			//Load current entry
			std::size_t keySize = 0;
			if (!a_intfc->ReadRecordData(&keySize, sizeof(keySize))) { return false; }
			std::string key;
			key.resize(keySize);
			if (!a_intfc->ReadRecordData(key.data(), static_cast<std::uint32_t>(keySize))) { return false; }
			logger::info("Key: {}", key);

			bool bExistsInMap = newspaperMap.contains(key);

			RE::FormID a_bookID;
			if (!Utility::ReadFormID(a_intfc, a_bookID)) {
				logger::warn("Failed to resolve formID. Plugin may have been removed");
				return false;
			}
			logger::info("Read formID: {}", a_bookID);

			//Load book description
			std::size_t strSize = 0;
			if (!a_intfc->ReadRecordData(strSize)) { return false; }
			std::string CNAM;
			CNAM.resize(strSize);
			if (!a_intfc->ReadRecordData(CNAM.data(), static_cast<std::uint32_t>(strSize))) { 
				return false; 
			}

			//Load lastUpdatedDay
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