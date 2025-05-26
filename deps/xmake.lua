-- includes("kananlib")

-- add_repositories("third-party kananlib", { rootdir = os.scriptdir() })


add_requires("safetyhook", { debug = is_mode_debug(), configs = {runtimes = get_mode_runtimes()}})

add_repositories("mymods-deps shared-mymods-deps")

add_requires("spdlog v1.12.0", { debug = is_mode_debug(), configs = {runtimes = get_mode_runtimes()}})
add_requires("bddisasm v1.37.0", { debug = is_mode_debug(), configs = {runtimes = get_mode_runtimes()}})
add_requires("kananlib v1.0", { debug = is_mode_debug(), configs = {runtimes = get_mode_runtimes()}})
