#include "ConditionManager.h"
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
			logger::info("{} file(s) found for key {}", entries.size(), key);

			std::string buffer;
			std::vector<entryFormat> tmp_entries;

			for (const auto& filepath : entries) {
				logger::info("Reading {}", filepath);

				auto ec = glz::read_file_json(tmp_entries, filepath, buffer);
				if (ec) {
					auto err = glz::format_error(ec, buffer);
					logger::error("{}", err);
				}
				else {
					AppendUniqueEntries(tmp_entries, DataManager::newspaperMap.at(name));
				}

				buffer.clear();
				tmp_entries.clear();
			}

			logger::info("{} generic and {} conditioned entries found", 
				newspaper.genericEntries.size(), newspaper.conditionedEntries.size());

		}

	}

	void AppendUniqueEntries(std::vector<entryFormat>& tmp_entries, Newspaper& newspaper)
	{
		for (const auto& tmp_entry : tmp_entries) {
			//Check if entry has already been used
			const auto textHash = clib_util::hash::fnv1a_32<std::string>(tmp_entry.value);
			if (DataManager::usedEntrySet.contains(textHash)) {
				continue;
			}

			//Generic entry (no conditions)
			if (!tmp_entry.conditions) {
				newspaper.genericEntries.emplace_back(
					Newspaper::genericEntry{ tmp_entry.title, tmp_entry.value }
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
					logger::warn("Invalid formID, skipping entry '{}'", tmp_entry.title);
					valid = false;
					break;
				}
			}
			if (valid) {
				entry.title = tmp_entry.title;
				entry.value = tmp_entry.value;
				entry.playerAllegiance = tmp_entry.conditions->playerAllegiance;
				newspaper.conditionedEntries.push_back(std::move(entry));
			}
		}
	}
}