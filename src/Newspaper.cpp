#include "Newspaper.h"
#include "DataManager.h"
#include "Utility.h"


bool TestConditions(const Newspaper::conditionedEntry& cEntry)
{
	logger::info("Testing entry: '{}'", cEntry.title);
	const int CWAllegiance = Utility::GetCWAllegiance();

	//Test player allegiance
	if (cEntry.playerAllegiance && cEntry.playerAllegiance.value() == CWAllegiance) {
		logger::info("player allegiance {} doesn't match actual {}", cEntry.playerAllegiance.value(), CWAllegiance);
		return false;
	}

	//Evaluate all conditions using AND
	for (const auto& condition : cEntry.questStages) {
		if (!Utility::ValidateQuestCondition(condition)) {
			return false;
		}
		//Valid condition
		continue;
	}

	return true;
}

//Distribute newspaper base form to containers
void Newspaper::DistributeToContainers(std::vector<std::string> containerIDs) 
{
	for (auto formStr : containerIDs) {
		auto container = Utility::GetFormFromString<RE::TESContainer>(formStr);
		if (!container) {
			logger::warn("Skipping {} - container not found", formStr);
			continue;
		}
		
		const auto boundOBJ = bookOBJ->As<RE::TESBoundObject>();
		container->RemoveObjectFromContainer(boundOBJ, 1);
		container->AddObjectToContainer(boundOBJ, 1, nullptr);
	}
}

//Return formatted entry
const std::string Newspaper::FormatNewEntry(const std::string& title, const std::string& value)
{
	//TEMPORARY
	const std::string bookText = std::format("{}\n\n{}", title, value);
	return bookText;
}

//Find next article
void Newspaper::UpdateEntry()
{
	//Evaluate all conditioned entries
	for (auto it = conditionedEntries.begin(); it != conditionedEntries.end(); ++it) {
		const auto& cEntry = *it;
		if (TestConditions(cEntry)) {
			const auto& bookText = FormatNewEntry(cEntry.title, cEntry.value);
			Utility::ReplaceBookContents(bookOBJ, bookText);

			const auto textHash = clib_util::hash::fnv1a_32<std::string>(cEntry.value);
			DataManager::usedEntrySet.insert(textHash);
			conditionedEntries.erase(it);
			return;
		}
	}

	//No valid conditioned entries found
	if (genericEntries.size() == 0) { return; }
	const auto index = clib_util::RNG().generate<std::size_t>(0, genericEntries.size() - 1);
	logger::info("Got object at index {} of {}", index, genericEntries.size() - 1);

	const auto& gEntry = genericEntries.at(index);
	const auto& bookText = FormatNewEntry(gEntry.title, gEntry.value);
	logger::info("Found entry {}", gEntry.title);
	Utility::ReplaceBookContents(bookOBJ, bookText);

}