#pragma once

namespace LookupConfigs
{
	struct configFormat {
		std::string key;
		std::string bookName1;
		std::string bookName2;
		std::vector<int> formlists;
		std::string worldspace;
	};

	static std::vector<configFormat> configs;
	static std::unordered_set<std::string> existing_keys;

	void ReadConfigs();
	void AppendUniqueConfigs(std::vector<configFormat> &tmp_configs);

}