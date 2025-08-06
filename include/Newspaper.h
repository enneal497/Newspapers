#pragma once

class Newspaper
{
public:
	Newspaper(float tmp_updateInterval)
		: updateInterval(tmp_updateInterval) {
	}

	float updateInterval;
	float lastUpdatedDay = 0;

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
	std::vector<RE::TESContainer*> containerPtrs;

	void ResolveContainers(const std::vector<std::string> containerIDs);
	void UpdateContainers(RE::TESBoundObject* boundOBJ);
	void PushNewEntry(RE::FormID formID);
	void UpdateEntry();

};