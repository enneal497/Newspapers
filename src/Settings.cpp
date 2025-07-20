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

        logger::info("Loaded settings");
        logger::info("");
    }
}