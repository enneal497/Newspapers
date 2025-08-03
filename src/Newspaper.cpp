#include "Newspaper.h"
#include "Utility.h"

//Distribute newspaper base form to containers
void Newspaper::DistributeToContainers(std::vector<std::string> containerIDs) {
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