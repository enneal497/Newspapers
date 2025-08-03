#pragma once
#include "Newspaper.h"

namespace LookupEntries
{
	/*
	struct conditionFormat {
		std::string var;
		char op;
		int value;
	};
	*/
	struct entryConditionsFormat {
		std::optional<int> playerAllegiance;
		std::vector<std::tuple<std::string, char, int>> questStages;
	};
	struct entryFormat {
		std::string title;
		std::string value;
		std::optional<entryConditionsFormat> conditions;
	};

	void ReadEntriesFromFile();
	void ProcessUniqueEntries(std::vector<entryFormat>& tmp_entries, Newspaper& newspaper);

}