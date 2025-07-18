#include "LookupConfigs.h"

namespace LookupConfigs
{
	void ReadConfigs()
	{
		std::filesystem::path dir{ R"(Data\SKSE\Newspapers\Config\)" };
		if (std::error_code ec; !std::filesystem::exists(dir, ec)) {
			std::string error_message = ec.message();
			logger::critical("Data/SKSE/Newspapers/Config not found ({})", error_message);
			return;
		}

		for (const auto& entry : std::filesystem::directory_iterator(dir)) {
			if (entry.path().extension() != ".json"sv) {
				continue;
			}
			std::string filepath = entry.path().string();
			logger::info("Reading {}", filepath);

			std::string buffer;
			std::vector<configFormat> tmp_configs;
			auto ec = glz::read_file_json(tmp_configs, filepath, buffer);

			if (ec) {
				auto err = glz::format_error(ec, buffer);
				logger::error("{}", err);
			}
			else {
				logger::info("Read {} entries", tmp_configs.size());
				AppendUniqueConfigs(tmp_configs);
			}
		}

		logger::info("Read {} unique configs", existing_keys.size());
	}

	void AppendUniqueConfigs(std::vector<configFormat>& tmp_configs)
	{
		for (const auto& tmp_cfg : tmp_configs) {
			if (existing_keys.insert(tmp_cfg.key).second) {
				//Create and store Newspaper object instead by key in a map
				configs.push_back(tmp_cfg);
			}
			else {
				logger::warn("Skipping {} - already exists", tmp_cfg.key);
			}
		}
	}
}