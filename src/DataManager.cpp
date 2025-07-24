#include "DataManager.h"
#include "LookupConfigs.h"
#include "Utility.h"

namespace DataManager
{
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
			//Save bookNames
			if (!a_intfc->WriteRecordData(value.GetBookOLD()->GetFormID()) ||
				!a_intfc->WriteRecordData(value.GetBookNEW()->GetFormID())) {
				return false;
			}
		}
		return true;
	}

	bool LoadConfigData(SKSE::SerializationInterface* a_intfc)
	{
		std::size_t mapSize;
		a_intfc->ReadRecordData(&mapSize, sizeof(mapSize));
		newspaperMap.clear();

		logger::info("mapSize: {}", mapSize);

		if (mapSize == 0) { return false; }

		for (; mapSize > 0; --mapSize) {
			std::string key;
			RE::FormID bookID1;
			RE::FormID bookID2;

			a_intfc->ReadRecordData(&key, sizeof(key));
			a_intfc->ReadRecordData(&bookID1, sizeof(bookID1));
			a_intfc->ReadRecordData(&bookID2, sizeof(bookID2));

			//Resolve formIDs
			if (!a_intfc->ResolveFormID(bookID1, bookID1)) {
				logger::warn("Failed to resolve formID {}", bookID1);
				continue;
			}
			if (!a_intfc->ResolveFormID(bookID2, bookID2)) {
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

		//Read and append formIDs from file
		LookupConfigs::ReadConfigsFromFile(false);

		return true;
	}
}