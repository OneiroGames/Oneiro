--
-- Copyright (c) Oneiro Games. All rights reserved.
-- Licensed under the GNU General Public License, Version 3.0.
--

includes("./Modules")

add_requires("flecs", {configs = {shared = true}})
add_requires("imgui v1.90.3-docking", {configs = {sdl2_opengl3 = true, wchar32 = true}})
add_requires("stb")
add_requires("glm")
add_requires("fmt", {configs = {shared = true}})
add_requires("spdlog", {configs = {shared = true, fmt_external = true}})
add_requires("physfs", {configs = {shared = true}})
add_requires("imguizmo")
add_requires("nameof")
add_requires("xxhash")
add_requires("rapidjson")

target("Oneiro-Common")
    set_languages("c++23")
    set_kind("shared")

    if not (get_config("arch") == nil) then
        set_targetdir("$(projectdir)/bin/" .. get_config("mode") .. "-" .. get_config("arch") .. "/")
    end

    add_includedirs("Runtime/Include/", {public = true})

    add_headerfiles("Runtime/Include/Oneiro/Common/StdAfx.hpp")
    add_files("Runtime/Include/Oneiro/Common/AssetsProviders/*.mpp", {public = true})
    add_files("Runtime/Include/Oneiro/Common/ECS/Components/*.mpp", {public = true})
    add_files("Runtime/Include/Oneiro/Common/ECS/*.mpp", {public = true})
    add_files("Runtime/Include/Oneiro/Common/RHI/*.mpp", {public = true})
    add_files("Runtime/Include/Oneiro/Common/WM/*.mpp", {public = true})
    add_files("Runtime/Include/Oneiro/Common/FileSystem/*.mpp", {public = true})
    add_files("Runtime/Include/Oneiro/Common/*.mpp", {public = true})
    
    add_files("Runtime/Source/Common/AssetsProviders/*.cpp")
    add_files("Runtime/Source/Common/ECS/Components/*.cpp")
    add_files("Runtime/Source/Common/ECS/*.cpp")
    add_files("Runtime/Source/Common/RHI/*.cpp")
    add_files("Runtime/Source/Common/WM/*.cpp")
    add_files("Runtime/Source/Common/FileSystem/*.cpp")
    add_files("Runtime/Source/Common/*.cpp")

    add_packages("flecs", "imgui", "glm", "fmt", "spdlog", "physfs", "nameof", "xxhash", "rapidjson", {public = true})

    add_defines("GLM_ENABLE_EXPERIMENTAL", "NOMINMAX", {public = true})

    add_syslinks("User32", "Advapi32", "Shell32", {public = true})

    after_build(function (target)
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

target("Oneiro-Core")
    set_languages("c++23")
    set_kind("shared")

    if not (get_config("arch") == nil) then
        set_targetdir("$(projectdir)/bin/" .. get_config("mode") .. "-" .. get_config("arch") .. "/")
    end

    add_includedirs("Runtime/Include/", {public = true})

    add_files("Runtime/Include/Oneiro/Core/*.mpp", {public = true})

    add_files("Runtime/Source/Core/*.cpp")

    add_deps("Oneiro-Common", {public = true})