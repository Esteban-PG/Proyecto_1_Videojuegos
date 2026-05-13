-- scene_tiled_01.lua  —  Nitro Rush: Tiled Level 01

current_level = "level_tiled_01"
next_level    = "level_02"
zone_points   = 100

-- ── HUD ───────────────────────────────────────────────────────────────────────
local E   = {}
local idx = 0

E[idx] = { components = {
    transform = { position={x=0,y=0}, scale={x=1,y=1}, rotation=0 },
    sprite    = { asset_id="hud_bar", width=800, height=40,
                  src_rect={x=0,y=0}, is_fixed=true },
}} ; idx = idx + 1

E[idx] = { components = {
    transform = { position={x=10,y=8}, scale={x=1,y=1}, rotation=0 },
    text  = { text="SCORE: 0", font_asset_id="font_hud",
              color={r=255,g=255,b=80,a=255}, is_fixed=true },
    script = { path="./assets/scripts/hud_score.lua" },
}} ; idx = idx + 1

E[idx] = { components = {
    transform = { position={x=440,y=8}, scale={x=1,y=1}, rotation=0 },
    text  = { text="NITRO: READY [SPC]", font_asset_id="font_hud",
              color={r=80,g=255,b=80,a=255}, is_fixed=true },
    script = { path="./assets/scripts/hud_nitro.lua" },
}} ; idx = idx + 1

-- ── ESCENA ────────────────────────────────────────────────────────────────────
scene = {
    sprites = {
        [0] = { assetId="car_yellow_sprite", filePath="./assets/images/car_yellow_1.png" },
    },
    color_textures = {
        [0] = { assetId="ramp_yellow", r=255, g=210, b=0,   a=255 },
        [1] = { assetId="gap_void",    r=5,   g=5,   b=10,  a=255 },
        [2] = { assetId="saw_red",     r=220, g=40,  b=40,  a=255 },
        [3] = { assetId="checkpoint",  r=240, g=240, b=240, a=255 },
        [4] = { assetId="hud_bar",     r=0,   g=0,   b=0,   a=180 },
    },
    fonts = {
        [0] = { assetId="font_hud",  filePath="./assets/fonts/press_start.ttf", fontSize=18 },
        [1] = { assetId="font_hud2", filePath="./assets/fonts/press_start.ttf", fontSize=14 },
    },
    keys = {
        [0] = { action="accelerate", key=119        },
        [1] = { action="accelerate", key=1073741906 },
        [2] = { action="turn_left",  key=97         },
        [3] = { action="turn_left",  key=1073741904 },
        [4] = { action="turn_right", key=100        },
        [5] = { action="turn_right", key=1073741903 },
        [6] = { action="nitro",      key=32         },
        [7] = { action="nitro",      key=1073742049 },
    },
    buttons  = {},
    map = {
        path = "./assets/map/lvl_01.tmx",
    },
    entities = E,
}
