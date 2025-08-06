#include "LookupEntries.h"
#include "DataManager.h"
#include "Utility.h"

namespace LookupEntries
{
	void ReadEntriesFromFile()
	{
		auto dir = R"(Data\SKSE\Newspapers\)";
		
		//Iterate over available keys
		for (const auto& [name, newspaper] : DataManager::newspaperMap) {
			const std::string& key = "_" + name;
			const auto& entries = clib_util::distribution::get_configs(dir, key, ".json");
			logger::debug("{} file(s) found for key {}", entries.size(), key);

			std::string buffer;
			std::vector<entryFormat> tmp_entries;

			for (const auto& filepath : entries) {
				logger::debug("Reading {}", filepath);

				auto ec = glz::read_file_json(tmp_entries, filepath, buffer);
				if (ec) {
					auto err = glz::format_error(ec, buffer);
					logger::error("{}", err);
				}
				else {
					ProcessUniqueEntries(tmp_entries, DataManager::newspaperMap.at(name));
				}

				buffer.clear();
				tmp_entries.clear();
			}

			logger::info("{} generic and {} conditioned entries found for key {}", 
				newspaper.genericEntries.size(), newspaper.conditionedEntries.size(), key);

		}

	}

	//Validate found entries
	void ProcessUniqueEntries(std::vector<entryFormat>& tmp_entries, Newspaper& newspaper)
	{
		//Reset existing sets
		newspaper.genericEntries.clear();
		newspaper.conditionedEntries.clear();

		for (const auto& tmp_entry : tmp_entries) {
			//Check if entry has already been used
			const auto tmp_formID = Utility::GetFormIDFromString(tmp_entry.bookID);
			if (tmp_formID == 0) { 
				logger::info("Unable to resolve formID {}", tmp_entry.bookID);
				continue; 
			}

			if (DataManager::usedEntrySet.contains(tmp_formID)) {
				continue;
			}

			//Generic entry (no conditions)
			if (!tmp_entry.conditions) {
				newspaper.genericEntries.emplace_back(
					Newspaper::genericEntry{ tmp_formID }
				);
				continue;
			}
			//Conditioned entry
			bool valid = true;
			Newspaper::conditionedEntry entry;
			for (const auto& questCondition : tmp_entry.conditions->questStages) {
				auto [varStr, op, value] = questCondition;
				if (const auto* var = RE::TESForm::LookupByEditorID(varStr); var) {
					//Valid condition
					entry.questStages.emplace_back(
						Newspaper::conditionFormat{ var->formID, op, value }
					);
				}
				else {
					//Invalid condition
					logger::warn("Invalid formID, skipping entry '{}'", tmp_formID);
					valid = false;
					break;
				}
			}
			if (valid) {
				entry.formID = tmp_formID;
				entry.playerAllegiance = tmp_entry.conditions->playerAllegiance;
				newspaper.conditionedEntries.push_back(std::move(entry));
			}
		}
	}
}