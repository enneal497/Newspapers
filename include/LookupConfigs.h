#pragma once

namespace LookupConfigs
{
	struct configFormat {
		std::string key;
		std::string bookName1;
		std::string bookName2;
		std::vector<std::string> formlists;
	};

	void ReadConfigsFromFile();
	void AppendUniqueConfigs(std::vector<configFormat> &tmp_configs);

}