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

		logger::debug("Saving data ...");
		if (!a_intfc->OpenRecord(sEntries, sVersion)) {
			logger::critical("Unable to open record sEntries to write cosave data");
			return;
		}
		if (!DataManager::SaveEntryData(a_intfc)) {
			logger::critical("Failed to save data to cosave");
			return;
		}
		logger::debug("Finished saving data");
		logger::debug("");
	}

	//Load cosave data to DataManager::newspaperMap
	void Manager::OnGameLoaded(SKSE::SerializationInterface* a_intfc)
	{
		std::scoped_lock<std::shared_mutex> locker(_lock);
		logger::info("Loading data ...");
		std::uint32_t type, version, length;

		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			if (version < sVersion) {
				logger::critical("Cosave data is incompatible with this version of the mod.");
				return;
			}
			if (type == sEntries) {
				if (!DataManager::LoadEntryData(a_intfc)) {
					logger::critical("Failed to read data from cosave");
					return;
				}
			}
		}

		logger::info("Finished loading data");
		logger::info("");

		//Read entries from file after data is loaded
		//LookupEntries::ReadEntriesFromFile();
		//LookupConfigs::ReadConfigsFromFile();
		Utility::TimeFunction("ReadConfigs", LookupConfigs::ReadConfigsFromFile);
		Utility::TimeFunction("ReadEntries", LookupEntries::ReadEntriesFromFile);
	}

	void Manager::OnRevert(SKSE::SerializationInterface*)
	{
		std::scoped_lock<std::shared_mutex> locker(_lock);
		DataManager::newspaperMap.clear();
	}
}