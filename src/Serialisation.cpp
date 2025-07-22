#include "Serialisation.h"
#include "DataManager.h"

namespace Serialisation
{
	void InitialiseSerialisation()
	{
		logger::info("Initialising cosave serialisation");
		const auto serialisation = SKSE::GetSerializationInterface();
		serialisation->SetUniqueID(sUID);
		serialisation->SetSaveCallback(OnGameSaved);
		serialisation->SetLoadCallback(OnGameLoaded);
		serialisation->SetRevertCallback(OnRevert);
		logger::info("Cosave serialisation initialized.");

	}

	//Save contents of DataManager::newspaperMap
	void OnGameSaved(SKSE::SerializationInterface* serialisation)
	{
		if (!serialisation->OpenRecord(sConfigs, sVersion)) {
			logger::critical("Unable to open record sConfigs to write cosave data");
			return;
		}
		if (!DataManager::SaveConfigData(serialisation)) {
			logger::critical("Failed to save data to cosave");
		}
	}

	//Load cosave data to DataManager::newspaperMap
	void OnGameLoaded(SKSE::SerializationInterface* serialisation)
	{
		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;
		while (serialisation->GetNextRecordInfo(type, version, length)) {
			if (version < sVersion) {
				logger::critical("Cosave data is incompatible with this version of the mod.");
				break;
			}
			//Load config data
			if (type == sConfigs) {
				if (!DataManager::LoadConfigData(serialisation)) {
					logger::critical("Failed to read data from cosave");
				}
			}
		}
	}

	void OnRevert(SKSE::SerializationInterface*)
	{
		DataManager::newspaperMap.clear();
	}
}