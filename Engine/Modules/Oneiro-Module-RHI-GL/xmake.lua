--
-- Copyright (c) Oneiro Games. All rights reserved.
-- Licensed under the GNU General Public License, Version 3.0.
--

target("Oneiro-Module-RHI-GL")
    set_languages("c++23")
    set_kind("shared")

    if not (get_config("arch") == nil) then
        set_targetdir("$(projectdir)/bin/" .. get_config("mode") .. "-" .. get_config("arch") .. "/Modules/Oneiro-Module-RHI-GL/")
    end

    add_deps("Oneiro-Common", "glad", {public = true})

    add_includedirs("Include/", {public = true})
    add_files("Include/*.mpp", {public = true})
    add_files("Source/*.cpp")
    add_files("Source/Fwog/*.cpp")
    add_files("Source/Fwog/detail/*.cpp")

    after_build(function (target)
        os.cp("$(scriptdir)/module.json", target:targetdir())
    end)

    