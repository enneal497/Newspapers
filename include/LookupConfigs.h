#pragma once

namespace LookupConfigs
{
	struct configFormat {
		std::string key;
		float updateInterval;
		std::vector<std::string> containers;
	};

	inline const std::filesystem::path dir = R"(Data\SKSE\Newspapers\Config\)";

	void ReadConfigsFromFile();
	void AppendUniqueConfigs(std::vector<configFormat> &tmp_configs);

}