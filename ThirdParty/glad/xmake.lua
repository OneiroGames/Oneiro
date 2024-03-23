--
-- Copyright (c) Oneiro Games. All rights reserved.
-- Licensed under the GNU General Public License, Version 3.0.
--

target("glad")
    set_languages("c++11")
    set_kind("static")

    if not (get_config("arch") == nil) then
        set_targetdir("$(projectdir)/bin/" .. get_config("mode") .. "-" .. get_config("arch") .. "/")
    end

    add_includedirs("include/", {public = true})
    add_files("src/*.c")