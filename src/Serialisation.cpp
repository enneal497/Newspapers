#include "Serialisation.h"
#include "LookupEntries.h"
#include "LookupConfigs.h"
#include "DataManager.h"
#include "Utility.h"

//TODO Handle loading new installation on existring save
namespace Serialisation
{
	void InitialiseSerialisation()
	{
		logger::info("Initialising cosave serialisation");
		const auto a_intfc = SKSE::GetSerializationInterface();
		a_intfc->SetUniqueID(sUID);
		a_intfc->SetSaveCallback(SaveCallback);
		a_intfc->SetLoadCallback(LoadCallback);
		a_intfc->SetRevertCallback(RevertCallback);
		logger::info("Cosave serialisation initialized.");
		logger::info("");
	}

	void SaveCallback(SKSE::SerializationInterface* a_intfc) { 
		Manager::GetSingleton()->OnGameSaved(a_intfc); 
	}
	void LoadCallback(SKSE::SerializationInterface* a_intfc) { 
		Manager::GetSingleton()->OnGameLoaded(a_intfc); 
	}
	void RevertCallback(SKSE::SerializationInterface* a_intfc) { 
		Manager::GetSingleton()->OnRevert(a_intfc); 
	}

	//Save contents of DataManager::newspaperMap
	void Manager::OnGameSaved(SKSE::SerializationInterface* a_intfc)
	{
		std::scoped_lock<std::shared_mutex> locker(_lock);

		logger::info("Saving data ...");
		if (!a_intfc->OpenRecord(sUsedEntries, sVersion) || !DataManager::SaveUsedEntries(a_intfc)) {
			logger::critical("Failed to save sUsedEntries data to cosave");
			return;
		}
		if (!a_intfc->OpenRecord(sConfigs, sVersion) || !DataManager::SaveConfigData(a_intfc)) {
			logger::critical("Failed to save sConfigs data to cosave");
			return;
		}
		logger::info("Finished saving data");
		logger::info("");
	}

	//Load cosave data to DataManager::newspaperMap
	void Manager::OnGameLoaded(SKSE::SerializationInterface* a_intfc)
	{
		std::scoped_lock<std::shared_mutex> locker(_lock);
		logger::info("Loading data ...");
		std::uint32_t type, version, length;
		bool bLoadedConfigs = false;

		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			if (version < sVersion) {
				logger::critical("Cosave data is incompatible with this version of the mod.");
				return;
			}
			if (type == sUsedEntries) {
				if (!DataManager::LoadUsedEntries(a_intfc)) {
					logger::critical("Failed to read sUsedEntries data from cosave");
					return;
				}
				//Read entries from file after data is loaded
				//LookupEntries::ReadEntriesFromFile();
				//LookupConfigs::ReadConfigsFromFile();
				Utility::TimeFunction("ReadConfigs", LookupConfigs::ReadConfigsFromFile);
				Utility::TimeFunction("ReadEntries", LookupEntries::ReadEntriesFromFile);

				bLoadedConfigs = true;
			}
			else if (type == sConfigs) {
				if (!bLoadedConfigs) {
					logger::critical("Failed to read sUsedEntries data from cosave");
					logger::info("");
					return;
				}
				if (!DataManager::LoadConfigData(a_intfc)) {
					logger::critical("Failed to read sConfigs data from cosave");
					logger::info("");
					return;
				}
			}
		}

		//Installation mid-game
		if (!bLoadedConfigs) {
			logger::info("No data found - loading from disk");
			LookupConfigs::ReadConfigsFromFile();
			LookupEntries::ReadEntriesFromFile();
			DataManager::UpdateAllEntries(true);
		}

		logger::info("Finished loading data");
		logger::info("");
	}

	void Manager::OnRevert(SKSE::SerializationInterface*)
	{
		std::scoped_lock<std::shared_mutex> locker(_lock);
		logger::info("OnRevert called");
		DataManager::newspaperMap.clear();
		DataManager::usedEntrySet.clear();
	}
}