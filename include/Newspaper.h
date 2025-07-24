#pragma once

class Newspaper
{
private:
	const RE::TESObjectBOOK* bookOLD;
	const RE::TESObjectBOOK* bookNEW;
	std::vector<RE::BGSListForm*> formlists;
	std::vector<RE::TESContainer*> containers;

public:
	Newspaper(const RE::TESObjectBOOK* tmp_bookOLD, const RE::TESObjectBOOK* tmp_bookNEW)
		: bookOLD(tmp_bookOLD),
		  bookNEW(tmp_bookNEW) {
	}
	void AppendFormlists(std::vector<std::string> formlistIDs);
	void AppendContainers(std::vector<std::string> containerIDs);

	const RE::TESObjectBOOK* GetBookOLD() { return bookOLD; }
	const RE::TESObjectBOOK* GetBookNEW() { return bookNEW; }
	std::vector<RE::BGSListForm*> GetFormlists() { return formlists; }
	std::vector < RE::TESContainer*> GetContainers() { return containers; }
};