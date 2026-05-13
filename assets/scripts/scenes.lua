-- scenes.lua  —  Nitro Rush scene registry
-- Each entry maps a name to the Lua file that defines the scene.

scenes = {
    [0] = { name = "nitro_menu",     path = "./assets/scripts/scene_menu.lua"     },
    [1] = { name = "level_tiled_01", path = "./assets/scripts/scene_tiled_01.lua" },
    [2] = { name = "level_tiled_02", path = "./assets/scripts/scene_tiled_02.lua" },
    [3] = { name = "level_01",       path = "./assets/scripts/scene_level1.lua"   },
    [4] = { name = "level_02",       path = "./assets/scripts/scene_level2.lua"   },
    [5] = { name = "level_03",       path = "./assets/scripts/scene_level3.lua"   },
}
