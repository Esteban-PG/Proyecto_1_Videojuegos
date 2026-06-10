current_level = "platform_01"
next_level    = "platform_01"   -- TODO: cambiar a "platform_02" cuando exista

scene = {
    zoom = 2.0,
    sprites = {
        [0] = { assetId="player_idle",  filePath="./assets/images/player/idle.png"  },
        [1] = { assetId="player_walk",  filePath="./assets/images/player/walk.png"  },
        [2] = { assetId="player_death", filePath="./assets/images/player/death.png" },
        [3] = { assetId="player_hurt",  filePath="./assets/images/player/hurt.png"  },
    },
    color_textures = {
        [0] = { assetId="placeholder_wall",   r=180, g=100, b=40,  a=255 },
        [1] = { assetId="placeholder_spike",  r=200, g=50,  b=50,  a=255 },
        [2] = { assetId="placeholder_ground", r=80,  g=160, b=80,  a=255 },
    },
    fonts = {
        [0] = { assetId="font_hud", filePath="./assets/fonts/press_start.ttf", fontSize=18 },
    },
    keys = {
        [0] = { action="move_left",  key=97         },   -- A
        [1] = { action="move_left",  key=1073741904 },   -- flecha izq
        [2] = { action="move_right", key=100        },   -- D
        [3] = { action="move_right", key=1073741903 },   -- flecha der
        [4] = { action="jump",       key=32         },   -- Space
        [5] = { action="jump",       key=119        },   -- W
    },
    music = "music_level1",
    map = {
        path = "./assets/map/platform_01.tmx",
    },
    entities = {},
}
