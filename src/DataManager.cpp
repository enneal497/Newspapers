#include "DataManager.h"
#include "Utility.h"

namespace DataManager
{
	bool SaveConfigData(SKSE::SerializationInterface* serialisation)
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
			if (!serialisation->WriteRecordData(value.GetBookOLD()->GetFormID()) ||
				!serialisation->WriteRecordData(value.GetBookNEW()->GetFormID())) {
				return false;
			}
		}
		return true;
	}

	bool LoadConfigData(SKSE::SerializationInterface* serialisation)
	{
		logger::info("Starting DataManager::LoadConfigData()");

		std::size_t mapSize;
		serialisation->ReadRecordData(&mapSize, sizeof(mapSize));
		newspaperMap.clear();

		logger::info("mapSize: {}", mapSize);

		if (mapSize == 0) { return false; }

		for (; mapSize > 0; --mapSize) {
			std::string key;
			RE::FormID bookID1;
			RE::FormID bookID2;

			serialisation->ReadRecordData(&key, sizeof(key));
			serialisation->ReadRecordData(&bookID1, sizeof(bookID1));
			serialisation->ReadRecordData(&bookID2, sizeof(bookID2));

			//Resolve formIDs
			if (!serialisation->ResolveFormID(bookID1, bookID1)) {
				logger::warn("Failed to resolve formID {}", bookID1);
				continue;
			}
			if (!serialisation->ResolveFormID(bookID2, bookID2)) {
				logger::warn("Failed to resolve formID {}", bookID2);
				continue;
			}

			auto bookOLD = RE::TESForm::LookupByID<RE::TESObjectBOOK>(bookID1);
			auto bookNEW = RE::TESForm::LookupByID<RE::TESObjectBOOK>(bookID1);
			if (!bookOLD || !bookNEW) { logger::warn("Failed to find book form"); continue; }

			auto result = newspaperMap.try_emplace(key, bookOLD, bookNEW);
			if (!result.second) {
				logger::critical("Failed to read key");
				return false;
			}

		}

		return true;
	}
}