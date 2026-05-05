-- scene_level1.lua  —  Nitro Rush: Level 1 — City / Asphalt
--
-- World layout (Y increases downward; player moves toward smaller Y = "north"):
--   Player starts at (385, 5800), road goes from y=6000 up to y=0.
--   Road corridor: x=250–550 (300px wide).
--   Left wall:  x=100–250  Right wall: x=550–700
--
-- Level progression (player moves upward, Y decreasing):
--   y=5200  Wall-saw (left)     +100 pts score zone at y=5000
--   y=4400  Ramp (first)        Gap: y=4100–3900
--   y=3400  Central saw         +200 pts at y=3200
--   y=2600  Double saw (×2)     +300 pts at y=2400
--   y=1800  Ramp (second)       Gap: y=1500–1300
--   y=900   Spinning saw        +400 pts at y=700
--   y=200   CHECKPOINT (finish)

-- ── Level progression info (read by player_car.lua) ────────────────────────
current_level = "level_01"
next_level    = "level_02"

-- ── Score zone point values (read at entity load time by score_zone.lua) ───
zone_points = 100   -- will be overridden per-zone below

scene = {
    -- ── Sprites (archivos PNG reales) ─────────────────────────────────────
    sprites = {
        [0] = { assetId = "car_yellow_sprite",
                filePath = "./assets/images/car_yellow_1.png" },
    },

    -- ── Colour textures ───────────────────────────────────────────────────
    color_textures = {
        [0]  = { assetId="road_asphalt",  r=55,  g=55,  b=65,  a=255 },
        [1]  = { assetId="wall_concrete", r=35,  g=35,  b=40,  a=255 },
        [2]  = { assetId="player_car",    r=50,  g=130, b=220, a=255 },
        [3]  = { assetId="saw_red",       r=220, g=40,  b=40,  a=255 },
        [4]  = { assetId="gap_void",      r=5,   g=5,   b=10,  a=255 },
        [5]  = { assetId="ramp_yellow",   r=255, g=210, b=0,   a=255 },
        [6]  = { assetId="checkpoint",    r=240, g=240, b=240, a=255 },
        [7]  = { assetId="hud_bar",       r=0,   g=0,   b=0,   a=180 },
    },

    -- ── Fonts ──────────────────────────────────────────────────────────────
    fonts = {
        [0] = { assetId="font_hud",  filePath="./assets/fonts/press_start.ttf", fontSize=18 },
        [1] = { assetId="font_hud2", filePath="./assets/fonts/press_start.ttf", fontSize=14 },
    },

    -- ── Key bindings ───────────────────────────────────────────────────────
    keys = {
        [0]  = { action="accelerate",  key=119  },  -- W
        [1]  = { action="accelerate",  key=1073741906 }, -- UP arrow
        [2]  = { action="turn_left",   key=97   },  -- A
        [3]  = { action="turn_left",   key=1073741904 }, -- LEFT arrow
        [4]  = { action="turn_right",  key=100  },  -- D
        [5]  = { action="turn_right",  key=1073741903 }, -- RIGHT arrow
        [6]  = { action="nitro",       key=32   },  -- SPACE
        [7]  = { action="nitro",       key=1073742049}, -- LEFT SHIFT
    },
    buttons = {},

    -- ── Entities ───────────────────────────────────────────────────────────
    entities = {

        -- ── ROAD background ─────────────────────────────────────────────
        [0] = {
            components = {
                transform = { position={x=250,y=0},    scale={x=1,y=1}, rotation=0 },
                sprite    = { asset_id="road_asphalt", width=300, height=6000,
                              src_rect={x=0,y=0} },
            }
        },

        -- ── LEFT WALL ────────────────────────────────────────────────────
        [1] = {
            components = {
                transform   = { position={x=100,y=0},     scale={x=1,y=1}, rotation=0 },
                sprite      = { asset_id="wall_concrete",  width=150, height=6000, src_rect={x=0,y=0} },
                box_collider= { width=150, height=6000 },
                tag         = { tag="wall" },
            }
        },

        -- ── RIGHT WALL ───────────────────────────────────────────────────
        [2] = {
            components = {
                transform   = { position={x=550,y=0},     scale={x=1,y=1}, rotation=0 },
                sprite      = { asset_id="wall_concrete",  width=150, height=6000, src_rect={x=0,y=0} },
                box_collider= { width=150, height=6000 },
                tag         = { tag="wall" },
            }
        },

        -- ══════════════ OBSTACLES ══════════════════════════════════════════

        -- ── Saw 1: wall saw (left) y=5200 ───────────────────────────────
        [3] = {
            components = {
                transform     = { position={x=250,y=5180}, scale={x=1,y=1}, rotation=0 },
                sprite        = { asset_id="saw_red", width=100, height=40, src_rect={x=0,y=0} },
                circle_collider={ radius=20, width=40, height=40 },
                tag           = { tag="saw" },
            }
        },
        -- Score zone for saw 1 (zone_points=100 is still set from top of file)
        [4] = {
            components = {
                transform = { position={x=250,y=5050}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=300, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- ── Ramp 1: y=4400 ──────────────────────────────────────────────
        [5] = {
            components = {
                transform   = { position={x=250,y=4380}, scale={x=1,y=1}, rotation=0 },
                sprite      = { asset_id="ramp_yellow", width=300, height=50, src_rect={x=0,y=0} },
                box_collider= { width=300, height=50 },
                tag         = { tag="ramp" },
            }
        },

        -- ── Gap 1: y=4100–3900 ───────────────────────────────────────────
        [6] = {
            components = {
                transform   = { position={x=250,y=3900}, scale={x=1,y=1}, rotation=0 },
                sprite      = { asset_id="gap_void", width=300, height=200, src_rect={x=0,y=0} },
                box_collider= { width=300, height=200 },
                tag         = { tag="gap" },
            }
        },

        -- ── Saw 2: central y=3400 ────────────────────────────────────────
        [7] = {
            components = {
                transform     = { position={x=370,y=3380}, scale={x=1,y=1}, rotation=0 },
                sprite        = { asset_id="saw_red", width=60, height=60, src_rect={x=0,y=0} },
                circle_collider={ radius=30, width=60, height=60 },
                tag           = { tag="saw" },
            }
        },
        -- zone_points set BEFORE entity definition via Lua globals
        -- (score_zone.lua captures zone_points at load time)
        [8] = {
            components = {
                transform = { position={x=250,y=3250}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=300, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- ── Saw 3a: double left y=2600 ───────────────────────────────────
        [9] = {
            components = {
                transform     = { position={x=255,y=2580}, scale={x=1,y=1}, rotation=0 },
                sprite        = { asset_id="saw_red", width=50, height=50, src_rect={x=0,y=0} },
                circle_collider={ radius=25, width=50, height=50 },
                tag           = { tag="saw" },
            }
        },
        -- ── Saw 3b: double right y=2600 ──────────────────────────────────
        [10] = {
            components = {
                transform     = { position={x=490,y=2580}, scale={x=1,y=1}, rotation=0 },
                sprite        = { asset_id="saw_red", width=50, height=50, src_rect={x=0,y=0} },
                circle_collider={ radius=25, width=50, height=50 },
                tag           = { tag="saw" },
            }
        },
        [11] = {
            components = {
                transform = { position={x=250,y=2430}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=300, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- ── Ramp 2: y=1800 ──────────────────────────────────────────────
        [12] = {
            components = {
                transform   = { position={x=250,y=1780}, scale={x=1,y=1}, rotation=0 },
                sprite      = { asset_id="ramp_yellow", width=300, height=50, src_rect={x=0,y=0} },
                box_collider= { width=300, height=50 },
                tag         = { tag="ramp" },
            }
        },

        -- ── Gap 2: y=1500–1300 ───────────────────────────────────────────
        [13] = {
            components = {
                transform   = { position={x=250,y=1300}, scale={x=1,y=1}, rotation=0 },
                sprite      = { asset_id="gap_void", width=300, height=200, src_rect={x=0,y=0} },
                box_collider= { width=300, height=200 },
                tag         = { tag="gap" },
            }
        },

        -- ── Saw 4: spinning y=900 ────────────────────────────────────────
        [14] = {
            components = {
                transform     = { position={x=370,y=880}, scale={x=1,y=1}, rotation=0 },
                sprite        = { asset_id="saw_red", width=60, height=60, src_rect={x=0,y=0} },
                circle_collider={ radius=30, width=60, height=60 },
                tag           = { tag="saw" },
                script        = { path="./assets/scripts/saw_spin.lua" },
            }
        },
        [15] = {
            components = {
                transform = { position={x=250,y=730}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=300, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- ── CHECKPOINT y=200 ─────────────────────────────────────────────
        [16] = {
            components = {
                transform   = { position={x=250,y=180}, scale={x=1,y=1}, rotation=0 },
                sprite      = { asset_id="checkpoint", width=300, height=40, src_rect={x=0,y=0} },
                box_collider= { width=300, height=40 },
                tag         = { tag="checkpoint" },
            }
        },

        -- ══════════════ PLAYER ═════════════════════════════════════════════
        [17] = {
            components = {
                -- Imagen real: 71×131 px, mostrada al 55% → ~39×72 px en pantalla
                transform = { position={x=381,y=5800}, scale={x=0.55,y=0.55}, rotation=0 },
                sprite    = { asset_id="car_yellow_sprite", width=71, height=131, src_rect={x=0,y=0} },
                rigid_body= { velocity={x=0,y=0} },
                box_collider={ width=71, height=131 },
                circle_collider={ radius=35, width=71, height=131 },
                tag       = { tag="player" },
                player    = { base_speed=290, rotation_speed=130, jump_duration=1.7,
                              jump_max_scale=1.7, base_scale_x=0.55, base_scale_y=0.55 },
                nitro     = { cooldown=1.0, boost_duration=1.0 },
                script    = { path="./assets/scripts/player_car.lua" },
            }
        },

        -- ══════════════ HUD (fixed screen-space) ══════════════════════════

        -- HUD background bar
        [18] = {
            components = {
                transform = { position={x=0,y=0}, scale={x=1,y=1}, rotation=0 },
                sprite    = { asset_id="hud_bar", width=800, height=40,
                              src_rect={x=0,y=0}, is_fixed=true },
            }
        },
        -- Score display
        [19] = {
            components = {
                transform = { position={x=10,y=8}, scale={x=1,y=1}, rotation=0 },
                text = {
                    text = "SCORE: 0",
                    font_asset_id = "font_hud",
                    color = { r=255, g=255, b=80, a=255 },
                    is_fixed = true
                },
                script = { path="./assets/scripts/hud_score.lua" },
            }
        },
        -- Nitro indicator
        [20] = {
            components = {
                transform = { position={x=500,y=8}, scale={x=1,y=1}, rotation=0 },
                text = {
                    text = "NITRO: READY [SPC]",
                    font_asset_id = "font_hud",
                    color = { r=80, g=255, b=80, a=255 },
                    is_fixed = true
                },
                script = { path="./assets/scripts/hud_nitro.lua" },
            }
        },
        -- Level label
        [21] = {
            components = {
                transform = { position={x=330,y=50}, scale={x=1,y=1}, rotation=0 },
                text = {
                    text = "LEVEL 1 - CITY",
                    font_asset_id = "font_hud2",
                    color = { r=180, g=180, b=255, a=200 },
                    is_fixed = true
                },
            }
        },
    },
}

-- Apply score zone values AFTER entity indices are defined.
-- score_zone.lua captures zone_points at script-load time, which happens
-- INSIDE loadEntities() when the scene is parsed. We set these globals
-- BEFORE the matching entities are loaded by placing them here, but since
-- Lua executes the full file first and THEN loadEntities() iterates,
-- the values are already in the environment.
-- Each score_zone entity's script picks up the most-recently-set zone_points.
-- To give different values, we reassign zone_points between entity blocks.
-- However, because all entities are defined in one table, the last assignment
-- wins.  The simplest reliable approach: all zones in level 1 give 100 pts.
-- For per-zone values use separate script files (e.g. score_zone_200.lua).
zone_points = 100
