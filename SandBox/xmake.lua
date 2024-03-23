--
-- Copyright (c) Oneiro Games. All rights reserved.
-- Licensed under the GNU General Public License, Version 3.0.
--

set_languages("c++23")

target("SandBox")
    set_kind("binary")

    if not (get_config("arch") == nil) then
        set_targetdir("$(projectdir)/bin/" .. get_config("mode") .. "-" .. get_config("arch") .. "/")
    end
    
    add_includedirs("Include/", {public = true})
    add_files("Include/*.mpp", {public = true})
    add_files("Source/*.cpp")
    add_deps("Oneiro-Core")