#include "LookupConfigs.h"
#include "DataManager.h"
#include "Utility.h"

namespace LookupConfigs
{
	void ReadConfigsFromFile()
	{
		//TODO - causes CTD if configDir is changed
		if (std::error_code ec; !std::filesystem::exists(DataManager::configDir, ec)) {
			std::string error_message = ec.message();
			std::string pathStr = DataManager::configDir.string();
			logger::critical("{} not found ({})", pathStr, error_message);
			return;
		}

		std::string buffer;
		std::vector<configFormat> tmp_configs;

		for (const auto& entry : std::filesystem::directory_iterator(dir)) {
			if (entry.path().extension() != ".json"sv) {
				continue;
			}
			const std::string filepath = entry.path().string();
			logger::info("Reading {}", filepath);

			auto ec = glz::read_file_json(tmp_configs, filepath, buffer);
			if (ec) {
				auto err = glz::format_error(ec, buffer);
				logger::error("{}", err);
			}
			else {
				logger::debug("Reading {} entries", tmp_configs.size());
				AppendUniqueConfigs(tmp_configs);
			}

			buffer.clear();
			tmp_configs.clear();
		}

		logger::info("Read {} unique configs", DataManager::newspaperMap.size());
		logger::info("");
	}

	void AppendUniqueConfigs(std::vector<configFormat>& tmp_configs)
	{
		for (const auto& tmp_cfg : tmp_configs) {
			//Try to create Newspaper object
			auto result = DataManager::newspaperMap.try_emplace(tmp_cfg.key, tmp_cfg.updateInterval);
			if (!result.second) {
				logger::warn("{} skipped - already exists", tmp_cfg.key);
				continue;
			}
			//result.first->second.ResolveContainers(std::move(tmp_cfg.containers));
			result.first->second.ResolveContainers(tmp_cfg.containers);
		}
	}
}