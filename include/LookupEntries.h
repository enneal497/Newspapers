#pragma once

namespace LookupEntries
{
	//Consider removing std::optional from globalVars and questStages
	struct conditionFormat {
		std::optional<bool> playerAllegiance;
		std::optional<std::vector<std::vector<std::string>>> globalVars;
		std::optional<std::vector<std::vector<std::string>>> questStages;
	};
	struct entryFormat {
		std::string title;
		std::string description;
		std::string value;
		std::optional<conditionFormat> conditions;
	};

	void ReadEntriesFromFile();
}