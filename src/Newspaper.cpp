#include "Newspaper.h"
#include "Utility.h"

void Newspaper::AppendFormlists(std::vector<std::string> formlistIDs) {
	if (!formlists.empty()) { return; }

	//Get formlist pointers
	for (auto formStr : formlistIDs) {
		auto formlist = Utility::GetFormFromString<RE::BGSListForm>(formStr);
		if (formlist) {
			formlists.push_back(formlist);
		}
		else {
			logger::warn("Skipping {} - formlist not found", formStr);
		}
	}
}

void Newspaper::AppendContainers(std::vector<std::string> containerIDs) {
	if (!containers.empty()) { return; }

	//Get container pointers
	for (auto formStr : containerIDs) {
		auto container = Utility::GetFormFromString<RE::TESContainer>(formStr);
		if (container) {
			containers.push_back(container);
		}
		else {
			logger::warn("Skipping {} - container not found", formStr);
		}
	}

}