#pragma once

namespace LookupConfigs
{
	struct configFormat {
		std::string key;
		std::string bookID;
		std::vector<std::string> containers;
	};


	void ReadConfigsFromFile(const bool bNewGame);
	void AppendUniqueConfigs(std::vector<configFormat> &tmp_configs, const bool bNewGame);

}