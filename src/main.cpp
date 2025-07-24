#include "Settings.h"
#include "Utility.h"
#include "Serialisation.h"
#include "LookupConfigs.h"

void Listener(SKSE::MessagingInterface::Message* message) noexcept
{
    switch (message->type) {
    case SKSE::MessagingInterface::kDataLoaded:
        Settings::LoadSettings();
        Serialisation::InitialiseSerialisation();
        break;

    case SKSE::MessagingInterface::kNewGame:
        //To avoid errors, only load configs on new game
        //LookupConfigs::ReadConfigsFromFile();
        Utility::TimeFunction("ReadConfigs", LookupConfigs::ReadConfigsFromFile, true);

        //Reset entries on startup regardless of state
        //LookupEntries::ReadEntries();
        break;

    case SKSE::MessagingInterface::kPostLoadGame:
        //LookupEntries::ReadEntries();
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
