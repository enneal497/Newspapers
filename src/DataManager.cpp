#include "DataManager.h"
#include "LookupConfigs.h"
#include "Utility.h"

namespace DataManager
{
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
			RE::FormID a_oldBookID;
			RE::FormID a_newBookID;
			a_intfc->ReadRecordData(&a_oldBookID, sizeof(a_oldBookID));

			if (!a_intfc->ResolveFormID(a_oldBookID, a_newBookID)) {
				logger::info("Failed to resolve ID: {}", a_oldBookID);
				continue;
			}

			usedEntrySet.insert(a_newBookID);
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
				if (!bForceUpdate) { newspaper.lastUpdatedDay = daysPassed; }
				newspaper.UpdateEntry();
			}
		}
	}

}