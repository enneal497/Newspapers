#include "Settings.h"

namespace Settings
{
    void LoadSettings() noexcept
    {
        logger::info("Loading settings");

        CSimpleIniA ini;
        ini.SetUnicode();
        ini.LoadFile(R"(.\Data\SKSE\Plugins\Newspapers.ini)");

        //Load global settings
        clib_util::ini::get_value(ini, debug_logging, "General", "bDebug");
        if (debug_logging) {
            spdlog::set_level(spdlog::level::debug);
            logger::debug("Debug logging enabled");
        }

        logger::info("Loaded settings");
        logger::info("");
    }
}