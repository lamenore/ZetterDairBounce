package("kananlib")
    -- set_sourcedir(os.scriptdir())
    -- set_kind("static")

    add_urls("https://github.com/cursey/kananlib.git")
    add_urls("git@github.com:cursey/kananlib.git")

    add_versions("v1.0", "main")

    add_deps("cmake")

    local deps_map =
    {
        bddisasm    = "bddisasm v1.37.0",
        spdlog      = "spdlog v1.12.0"
    }

    for key, value in pairs(deps_map) do
        add_deps(value)
    end
    
    add_configs("nolog", {description = "Set spdlog logging.", default = false})
    add_configs("fetch-bddisasm", {description = "Fetch bddisasm.", default = false, type = "boolean"})
    add_configs("fetch-spdlog", {description = "Fetch spdlog.", default = false, type = "boolean"})
    
    add_includedirs("include")
    add_links("bddisasm", "spdlog", "shlwapi")

    add_cflags("/EHa")

    on_load(function (package)
        local nolog = package:config("nolog")
        package:add("includedirs", "/include")

        for key, value in pairs(deps_map) do
            if package:config(key) then
                package:add("deps", value)
            end
        end
        
        package:add("links", "bddisasm")
        package:add("links", "spdlog")
        package:add("links", "shlwapi")
    end)

    on_install(function (package)
        
        os.rm("*.cmake")
        io.replace("CMakeLists.txt", "CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR", "NOT CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR", {plain = true})
        -- io.replace("CMakeLists.txt", "target_link_libraries(kananlib PUBLIC", "", {plain = true})
        -- io.replace("CMakeLists.txt", "target_link_libraries(kananlib-nolog PUBLIC", "", {plain = true})
        io.replace("CMakeLists.txt", "bddisasm::bddisasm", "bddisasm", {plain = true})
        io.replace("CMakeLists.txt", "spdlog::spdlog", "spdlog", {plain = true})
        -- io.replace("CMakeLists.txt", "shlwapi\n)", "", {plain = true})

        local configs = {}

        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))

        
        local packagedeps = {}
        for key, value in pairs(deps_map) do
            local ppa = package:dep(key)
            if (ppa) then 
                table.insert(configs, "-D" .. key .. "_DIR=" .. ppa:installdir())
            end
            table.insert(packagedeps, key)
        end

        import("package.tools.cmake").install(package, configs, { packagedeps = packagedeps })

        os.cp("include", package:installdir())
        os.mv(package:installdir() .. "/include/*.hpp", package:installdir() .. "/include/utility/")

        
    end)

    on_test(function (package)
        -- assert(package:has_cxxincludes("utility/Module.hpp"))
    end)
package_end()


