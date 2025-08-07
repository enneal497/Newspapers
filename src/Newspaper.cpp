#include "Newspaper.h"
#include "DataManager.h"
#include "Utility.h"



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
		//TODO - Move min,max to DataManager.h
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
void Newspaper::PushNewEntry(RE::FormID formID, bool bResetFlags)
{
	//TODO - Log as debug and convert formID to hex
	logger::info("Pushing new entry {}", formID);

	auto bookOBJ = RE::TESForm::LookupByID<RE::TESObjectBOOK>(formID);
	if (bResetFlags && bookOBJ->IsRead()) {
		bookOBJ->data.flags.reset(RE::OBJ_BOOK::Flag::kHasBeenRead);
		bookOBJ->RemoveChange(RE::TESObjectBOOK::ChangeFlags::kRead);
	}
	/*
	static const auto calendar = RE::Calendar::GetSingleton();
	const auto dateStr = std::format("{}, {} {}, {}", 
		calendar->GetDayName(), calendar->GetDay(), calendar->GetMonthName(), calendar->GetYear()
	);
	const auto nameStr = newspaperName + " - " + dateStr;

	bookOBJ->SetFullName(nameStr.c_str());
	*/

	auto boundOBJ = bookOBJ->As<RE::TESBoundObject>();
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
		if (Utility::TestConditions(cEntry)) {
			PushNewEntry(cEntry.formID, false);

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
	PushNewEntry(gEntry.formID, true);

}