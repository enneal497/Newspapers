#include "Newspaper.h"
#include "DataManager.h"
#include "Utility.h"

//Evaluate all entry conditions
bool TestConditions(const Newspaper::conditionedEntry& cEntry)
{
	logger::info("Testing entry: '{}'", cEntry.formID);
	const int CWAllegiance = Utility::GetCWAllegiance();

	//Test player allegiance
	if (cEntry.playerAllegiance && cEntry.playerAllegiance.value() != CWAllegiance) {
		//logger::info("player allegiance {} doesn't match actual {}", cEntry.playerAllegiance.value(), CWAllegiance);
		return false;
	}

	//Evaluate all conditions using AND
	for (const auto& condition : cEntry.questStages) {
		if (!Utility::ValidateQuestCondition(condition)) {
			return false;
		}
		//Valid condition
		//continue;
	}

	return true;
}

void Newspaper::ResolveContainers(const std::vector<std::string> containerIDs)
{
	for (auto formStr : containerIDs) {
		auto container = Utility::GetFormFromString<RE::TESContainer>(formStr);
		if (!container) {
			logger::warn("Skipping {} - container not found", formStr);
			continue;
		}

		containerPtrs.push_back(container);

	}
}

//Distribute newspaper base form to containers
void Newspaper::UpdateContainers(RE::TESBoundObject* boundOBJ)
{
	logger::info("Pushing to {} containers", containerPtrs.size());
	for (auto container : containerPtrs) {
		bool added = false;
		int count = clib_util::RNG().generate<int>(1, 10);
		
		//If old item is in container, remove it
		if (currentEntry) {
			const auto currentCount = container->CountObjectsInContainer(currentEntry);
			if (currentCount > 0) { container->RemoveObjectFromContainer(currentEntry, currentCount); }
		}

		container->AddObjectToContainer(boundOBJ, count, nullptr);
	}

}

//Format and replace with new text
void Newspaper::PushNewEntry(RE::FormID formID)
{
	//TEMPORARY
	logger::info("Pushing new entry {}", formID);

	auto boundOBJ = RE::TESForm::LookupByID<RE::TESBoundObject>(formID);
	//RemoveFromContainers()
	UpdateContainers(boundOBJ);

	//Reset current entry
	currentEntry = boundOBJ;
	logger::info("Pushed new entry");
	logger::info("");
}

//Find next article
void Newspaper::UpdateEntry()
{
	//Evaluate all conditioned entries
	for (auto it = conditionedEntries.begin(); it != conditionedEntries.end(); ++it) {
		const auto& cEntry = *it;
		if (TestConditions(cEntry)) {
			PushNewEntry(cEntry.formID);

			DataManager::usedEntrySet.insert(cEntry.formID);
			conditionedEntries.erase(it);
			return;
		}
	}

	//No valid conditioned entries found
	if (genericEntries.empty()) { return; }
	size_t index = clib_util::RNG().generate<std::size_t>(0, genericEntries.size() - 1);
	logger::info("Got object at index {} of {}", index, genericEntries.size() - 1);

	const auto& gEntry = genericEntries.at(index);
	PushNewEntry(gEntry.formID);

}