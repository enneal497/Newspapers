#pragma once


class Newspaper
{
private:
	RE::TESObjectBOOK* bookOBJ;

public:
	Newspaper(RE::TESObjectBOOK* tmp_book, float tmp_updateInterval)
		: bookOBJ(tmp_book),
		updateInterval(tmp_updateInterval) {
	}

	float updateInterval;
	float lastUpdatedDay = 0;

	struct conditionFormat {
		RE::FormID var;
		char op;
		int value;
	};
	struct genericEntry {
		std::string title;
		std::string value;
	};
	struct conditionedEntry {
		std::string title;
		std::string value;
		std::optional<int> playerAllegiance;
		std::vector<conditionFormat> questStages;
	};

	std::vector<genericEntry> genericEntries;
	std::vector<conditionedEntry> conditionedEntries;

	void DistributeToContainers(std::vector<std::string> containerIDs);

	const RE::TESObjectBOOK* GetBook() { return bookOBJ; }
};