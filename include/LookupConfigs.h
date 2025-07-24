#pragma once

namespace LookupConfigs
{
	struct configFormat {
		std::string key;
		std::string bookID1;
		std::string bookID2;
		std::vector<std::string> formlists;
		std::vector<std::string> containers;
	};


	void ReadConfigsFromFile();
	void AppendUniqueConfigs(std::vector<configFormat> &tmp_configs);

}