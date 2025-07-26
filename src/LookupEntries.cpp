#include "LookupEntries.h"
#include "DataManager.h"

namespace LookupEntries
{
	void ReadEntriesFromFile()
	{
		//Redundant, if LookupConfigs() is called first
		/*
		std::filesystem::path dir{ R"(Data\SKSE\Newspapers\)" };
		if (std::error_code ec; !std::filesystem::exists(dir, ec)) {
			std::string error_message = ec.message();
			logger::critical("Data/SKSE/Newspapers/ not found ({})", error_message);
			return;
		}
		*/

		const auto dir = R"(Data\SKSE\Newspapers\)";
		
		//Iterate over available keys
		for (const auto pair : DataManager::newspaperMap) {
			const auto& key = "_" + pair.first;
			std::vector<std::string> entries = clib_util::distribution::get_configs(dir, key, ".json");
			logger::info("{} file(s) found for key {}", entries.size(), key);
		}

	}
}