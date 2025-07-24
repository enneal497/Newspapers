#include "Serialisation.h"
#include "DataManager.h"
#include "Utility.h"

void TestFunction()
{
	logger::info("Testing serialisation success");
	auto mapSize = DataManager::newspaperMap.size();
	logger::info("Mapsize: {}", mapSize);
	if (mapSize > 0) {
		auto impMap = DataManager::newspaperMap.at("IMP");
		auto contSize = impMap.GetContainers().size();
		logger::info("Container size: {}", contSize);
	}
}

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
		if (!a_intfc->OpenRecord(sConfigs, sVersion)) {
			logger::critical("Unable to open record sConfigs to write cosave data");
			return;
		}
		if (!DataManager::SaveConfigData(a_intfc)) {
			logger::critical("Failed to save data to cosave");
			return;
		}
		logger::info("Finished saving data");
		TestFunction();
		logger::info("");
	}

	//Load cosave data to DataManager::newspaperMap
	void Manager::OnGameLoaded(SKSE::SerializationInterface* a_intfc)
	{
		std::scoped_lock<std::shared_mutex> locker(_lock);

		logger::info("Loading data ...");
		std::uint32_t type;
		std::uint32_t version;
		std::uint32_t length;
		while (a_intfc->GetNextRecordInfo(type, version, length)) {
			if (version < sVersion) {
				logger::critical("Cosave data is incompatible with this version of the mod.");
				return;
			}
			//Load config data
			if (type == sConfigs && !DataManager::LoadConfigData(a_intfc)) {
				logger::critical("Failed to read data from cosave");
				return;
			}
		}
		logger::info("Finished loading data");
		logger::info("");

		TestFunction();
	}

	void Manager::OnRevert(SKSE::SerializationInterface*)
	{
		std::scoped_lock<std::shared_mutex> locker(_lock);
		DataManager::newspaperMap.clear();
	}
}