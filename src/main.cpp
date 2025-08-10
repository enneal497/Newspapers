#include "Utility.h"
#include "Events.h"
#include "Serialisation.h"
#include "LookupConfigs.h"
#include "LookupEntries.h"
#include "DataManager.h"

void Listener(SKSE::MessagingInterface::Message* message) noexcept
{
    switch (message->type) {
    case SKSE::MessagingInterface::kDataLoaded:
        Events::CellEventHandler::Register();
        Serialisation::InitialiseSerialisation();
        break;

    case SKSE::MessagingInterface::kNewGame:
        //Only load configs on new game
        //LookupConfigs::ReadConfigsFromFile();
        Utility::TimeFunction(1, "ReadConfigs", LookupConfigs::ReadConfigsFromFile);
        //LookupEntries::ReadEntriesFromFile();
        Utility::TimeFunction(1, "ReadEntries", LookupEntries::ReadEntriesFromFile);
        //DataManager::UpdateAllEntries(true);
        Utility::TimeFunction(1, "UpdateAllEntries", DataManager::UpdateAllEntries, true);

        break;
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);

	const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto name{ plugin->GetName() };
    const auto version{ plugin->GetVersion() };

    logger::init();
    logger::info("{} {} is loading...", name, version);

    if (const auto messaging{ SKSE::GetMessagingInterface() }; !messaging->RegisterListener(Listener)) {
        return false;
    }

    logger::info("{} has finished loading.", name);
    logger::info("");

	return true;
}
