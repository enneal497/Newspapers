#pragma once

namespace LookupConfigs
{
	struct configFormat {
		std::string key;
		std::string bookName1;
		std::string bookName2;
		std::vector<std::string> formlists;
	};

	static std::vector<configFormat> configs;
	static std::unordered_set<std::string> existing_keys;

	void ReadConfigsFromFile();
	void AppendUniqueConfigs(std::vector<configFormat> &tmp_configs);

}