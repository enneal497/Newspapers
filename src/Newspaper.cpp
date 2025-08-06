#include "Newspaper.h"
#include "DataManager.h"
#include "Utility.h"


bool TestConditions(const Newspaper::conditionedEntry& cEntry)
{
	logger::info("Testing entry: '{}'", cEntry.title);
	const int CWAllegiance = Utility::GetCWAllegiance();

	//Test player allegiance
	if (cEntry.playerAllegiance && cEntry.playerAllegiance.value() != CWAllegiance) {
		logger::info("player allegiance {} doesn't match actual {}", cEntry.playerAllegiance.value(), CWAllegiance);
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

//Distribute newspaper base form to containers
void Newspaper::DistributeToContainers(const std::vector<std::string> containerIDs) 
{
	for (auto formStr : containerIDs) {
		auto container = Utility::GetFormFromString<RE::TESContainer>(formStr);
		if (!container) {
			logger::warn("Skipping {} - container not found", formStr);
			continue;
		}
		
		bool added = false;
		int count = clib_util::RNG().generate<int>(1, 10);
		const auto boundOBJ = bookOBJ->As<RE::TESBoundObject>();
		
		//If item is already in container, set new count
		for (std::uint32_t i = 0; i < container->numContainerObjects; ++i) {
			if (const auto entry = container->containerObjects[i]; entry && entry->obj == boundOBJ) {
				entry->count = count;
				added = true;
				break;
			}
		}
		if (!added) { container->AddObjectToContainer(boundOBJ, count, nullptr); }
	}
}

//Format and replace with new text
void Newspaper::PushNewEntry(const std::string& title, const std::string& value)
{
	//TEMPORARY
	const std::string bookText = std::format("{}\n\n{}", title, value);
	logger::info("Pushing new entry {}", title);

	Utility::ReplaceBookContents(bookOBJ, bookText);
	if (bookOBJ->IsRead()) {
		bookOBJ->data.flags.reset(RE::OBJ_BOOK::Flag::kHasBeenRead);
		bookOBJ->RemoveChange(RE::TESObjectBOOK::ChangeFlags::kRead);
	}

	//const auto boundOBJ = bookOBJ->As<RE::TESBoundObject>();
	//static auto player = RE::PlayerCharacter::GetSingleton()->GetActorBase();

	

	/*
	for (std::uint32_t i = 0; i < player->numContainerObjects; ++i) {
		if (!player->containerObjects[i]) { continue; }
		logger::info("Object {}", player->containerObjects[i]->obj->GetName());
		if (const auto entry = player->containerObjects[i]; entry && entry->obj == boundOBJ) {
			logger::info("Found newspaper {} in player inventory - OPT1", bookOBJ->GetName());
			break;
		}
	}
	*/
}

//Find next article
void Newspaper::UpdateEntry()
{
	//Evaluate all conditioned entries
	for (auto it = conditionedEntries.begin(); it != conditionedEntries.end(); ++it) {
		const auto& cEntry = *it;
		if (TestConditions(cEntry)) {
			PushNewEntry(cEntry.title, cEntry.value);

			auto textHash = clib_util::hash::fnv1a_32<std::string>(cEntry.value);
			DataManager::usedEntrySet.insert(textHash);
			conditionedEntries.erase(it);
			return;
		}
	}

	//No valid conditioned entries found
	if (genericEntries.empty()) { return; }
	size_t index = clib_util::RNG().generate<std::size_t>(0, genericEntries.size() - 1);
	logger::info("Got object at index {} of {}", index, genericEntries.size() - 1);

	const auto& gEntry = genericEntries.at(index);
	PushNewEntry(gEntry.title, gEntry.value);

}