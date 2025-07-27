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
		for (const auto pair : DataManager::newspaperMap) {
			const auto& key = "_" + pair.first;
			auto& newspaper = DataManager::newspaperMap.at(pair.first);
			const auto& entries = clib_util::distribution::get_configs(dir, key, ".json");
			logger::info("{} file(s) found for key {}", entries.size(), key);

			std::string buffer;
			std::vector<entryFormat> tmp_entries;

			for (const auto& filepath : entries) {
				logger::info("Reading {}", filepath.c_str());

				auto ec = glz::read_file_json(tmp_entries, filepath.c_str(), buffer);
				if (ec) {
					auto err = glz::format_error(ec, buffer);
					logger::error("{}", err);
				}
				else {
					AppendUniqueEntries(tmp_entries, newspaper);
				}

				buffer.clear();
				tmp_entries.clear();
			}

			logger::info("{} generic and {} conditioned entries found", 
				newspaper.genericEntries.size(), newspaper.conditionedEntries.size());

		}

	}

	void AppendUniqueEntries(std::vector<entryFormat>& tmp_entries, auto& newspaper)
	{
		for (const auto& tmp_entry : tmp_entries) {
			//Check if entry has been used already
			auto textHash = clib_util::hash::fnv1a_32<std::string>(tmp_entry.value);
			if (DataManager::usedEntrySet.contains(textHash)) {
				continue;
			}
			//Check if entry is generic
			if (!tmp_entry.conditions) {
				Newspaper::genericEntry entry;
				entry.title = tmp_entry.title;
				entry.value = tmp_entry.value;
				newspaper.genericEntries.push_back(entry);
				continue;
			}
			//Entry has conditions
			Newspaper::conditionedEntry entry;
			for (const auto& questCondition : tmp_entry.conditions->questStages) {
				auto [varStr, op, value] = questCondition;
				if (const auto& var = RE::TESForm::LookupByEditorID(varStr); var) {
					//Valid condition
					Newspaper::conditionFormat condition;
					condition.var = var->formID;
					condition.op = op;
					condition.value = value;
					entry.questStages.push_back(condition);
				}
				else {
					//Invalid condition
					logger::warn("Invalid formID, skipping entry '{}'", tmp_entry.title);
					break;
				}
			}
			//Add data only if all conditions are validated
			entry.title = tmp_entry.title;
			entry.value = tmp_entry.value;
			entry.playerAllegiance = tmp_entry.conditions->playerAllegiance;
			newspaper.conditionedEntries.push_back(entry);
		}
	}
}