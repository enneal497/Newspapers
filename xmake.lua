-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibsse-ng")
includes("lib/CLibUtil")

-- set project
set_project("Newspapers")
set_version("0.0.0")
set_license("GPL-3.0")

-- set defaults
set_plat("windows")
set_arch("x64")
set_languages("c++23")
set_warnings("allextra")
set_defaultmode("releasedbg")

-- set policies
set_policy("package.requires_lock", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- require packages
add_requires("glaze", "xbyak", "simpleini")

-- targets
target("Newspapers")
    -- add dependencies to target
    add_deps("commonlibsse-ng", "CLibUtil")

    -- add packages to target
    add_packages("glaze", "xbyak", "simpleini")

    -- add commonlibsse-ng plugin
    add_rules("commonlibsse-ng.plugin", {
        name = "Newspapers",
        author = "Enneal",
        description = "SKSE64 plugin template using CommonLibSSE-NG"
    })

    add_defines("UNICODE", "_UNICODE")

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("include/**.h")
    add_includedirs("include")
    set_pcxxheader("include/pch.h")

    -- copy files to SKSE/Plugins/
    after_build(function (target)
        local game_path = os.getenv("XSE_TES5_GAME_PATH")
        if not game_path then
            return
        end

        local config_dir = "config"
        if not os.isdir(config_dir) then
            return
        end

        local dest_dir = path.join(game_path, "Data", "SKSE", "Plugins")
        if not os.isdir(dest_dir) then
            return
        end

        os.cp(path.join(config_dir, "*"), dest_dir)
        print("Copied config files to: " .. dest_dir)
    end)
