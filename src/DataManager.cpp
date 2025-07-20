#include "DataManager.h"
#include "Utility.h"

Newspaper::Newspaper(LookupConfigs::configFormat& configData) {
	key = configData.key;
	bookOLD = configData.bookName1;
	bookNEW = configData.bookName2;

	//Get formlist pointers
	//TODO: Move function to Utility as template
	for (auto formStr : configData.formlists) {
		auto formlist = Utility::GetFormFromID<RE::BGSListForm>(formStr);
		if (formlist) {
			formlists.push_back(formlist);
		}
		else {
			logger::warn("Skipping {} - formlist not found", formStr);
		}
	}
}