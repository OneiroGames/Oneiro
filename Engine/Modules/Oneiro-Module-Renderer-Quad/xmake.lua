--
-- Copyright (c) Oneiro Games. All rights reserved.
-- Licensed under the GNU General Public License, Version 3.0.
--

target("Oneiro-Module-Renderer-Quad")
    set_languages("c++23")
    set_kind("shared")

    if not (get_config("arch") == nil) then
        set_targetdir("$(projectdir)/bin/" .. get_config("mode") .. "-" .. get_config("arch") .. "/Modules/Oneiro-Module-Renderer-Quad/")
    end

    add_deps("Oneiro-Common", {public = true})

    add_includedirs("Include/", {public = true})
    add_files("Include/*.mpp", {public = true})
    add_files("Source/*.cpp")

    after_build(function (target)
        os.cp("$(scriptdir)/module.json", target:targetdir())
        os.cp("$(scriptdir)/Shaders/", target:targetdir())
    end)

    add_extrafiles("xmake.lua")