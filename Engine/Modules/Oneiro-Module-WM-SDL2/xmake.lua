--
-- Copyright (c) Oneiro Games. All rights reserved.
-- Licensed under the GNU General Public License, Version 3.0.
--

add_requires("libsdl", {configs = {shared = true}})

target("Oneiro-Module-WM-SDL2")
    set_languages("c++23")
    set_kind("shared")

    if not (get_config("arch") == nil) then
        set_targetdir("$(projectdir)/bin/" .. get_config("mode") .. "-" .. get_config("arch") .. "/Modules/Oneiro-Module-WM-SDL2/")
    end

    add_packages("libsdl", {public = true})

    add_deps("Oneiro-Common", {public = true})

    add_includedirs("Include/", {public = true})
    add_files("Include/*.mpp", {public = true})
    add_files("Source/*.cpp")

    add_defines("SDL_MAIN_HANDLED", {public = true})

    after_build(function (target)
        os.cp("$(scriptdir)/module.json", target:targetdir())

        if (is_plat("windows")) then
            for _, pkg in ipairs(target:orderpkgs()) do 
                for _, dllpath in ipairs(table.wrap(pkg:get("libfiles"))) do
                    if dllpath:endswith(".dll") then
                        os.vcp(dllpath, target:targetdir()) 
                    end 
                end
            end
        end
    end)

    