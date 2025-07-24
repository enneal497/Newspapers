#include "LookupConfigs.h"
#include "DataManager.h"
#include "Utility.h"

namespace LookupConfigs
{
	void ReadConfigsFromFile(const bool bNewGame)
	{
		std::filesystem::path dir{ R"(Data\SKSE\Newspapers\Config\)" };
		if (std::error_code ec; !std::filesystem::exists(dir, ec)) {
			std::string error_message = ec.message();
			logger::critical("Data/SKSE/Newspapers/Config not found ({})", error_message);
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
				logger::info("Read {} entries", tmp_configs.size());
				AppendUniqueConfigs(tmp_configs, bNewGame);
			}

			buffer.clear();
			tmp_configs.clear();
		}

		logger::info("");
		logger::info("Read {} unique configs", DataManager::newspaperMap.size());
	}

	void AppendUniqueConfigs(std::vector<configFormat>& tmp_configs, const bool bNewGame)
	{
		for (const auto& tmp_cfg : tmp_configs) {
			auto bookID1 = Utility::GetFormFromString<RE::TESObjectBOOK>(tmp_cfg.bookID1);
			if (!bookID1) {
				logger::warn("Book FormID {} is invalid", tmp_cfg.bookID1);
				continue;
			}
			auto bookID2 = Utility::GetFormFromString<RE::TESObjectBOOK>(tmp_cfg.bookID2);
			if (!bookID2) {
				logger::warn("Book FormID {} is invalid", tmp_cfg.bookID2);
				continue;
			}

			if (bNewGame) {
				//Try to create Newspaper object
				auto result = DataManager::newspaperMap.try_emplace(tmp_cfg.key, bookID1, bookID2);
				if (!result.second) {
					logger::warn("{} skipped - already exists", tmp_cfg.key);
					continue;
				}

				result.first->second.AppendFormlists(tmp_cfg.formlists);
				result.first->second.AppendContainers(tmp_cfg.containers);
			}
			else {
				auto it = DataManager::newspaperMap.find(tmp_cfg.key);
				if (it != DataManager::newspaperMap.end()) {
					it->second.AppendFormlists(tmp_cfg.formlists);
					it->second.AppendContainers(tmp_cfg.containers);
				}
			}

		}
	}
}