#pragma once

class Newspaper
{
private:
	std::string newspaperName;

public:
	Newspaper(std::string tmp_name, float tmp_updateInterval)
		: updateInterval(tmp_updateInterval),
		newspaperName(tmp_name),
		currentEntry(nullptr), currentCNAM("") {
	}

	float updateInterval;
	int32_t lastUpdatedDay = -1;

	struct conditionFormat {
		RE::FormID formID;
		char op;
		int value;
	};

	//If nothing is added later, could be replaced by an std::vector<RE::FormID>
	struct genericEntry {
		RE::FormID formID;
	};
	struct conditionedEntry {
		RE::FormID formID;
		std::optional<int> playerAllegiance;
		std::vector<conditionFormat> questStages;
	};

	std::vector<genericEntry> genericEntries;
	std::vector<conditionedEntry> conditionedEntries;

	RE::TESBoundObject* currentEntry;
	std::string currentCNAM;
	std::vector<RE::TESContainer*> containerPtrs;

	void ResolveContainers(const std::vector<std::string> containerIDs);
	void UpdateContainers(RE::TESBoundObject* boundOBJ);
	void PushNewEntry(RE::FormID formID, bool bResetFlags, std::string CNAM = "");
	void UpdateEntry();

};