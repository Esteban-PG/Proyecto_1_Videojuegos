-- scene_level3.lua  —  Nitro Rush: Level 3 — Factory / Night
-- Dificultad alta. Camino estrecho (240px), visibilidad reducida (fondo oscuro),
-- secuencias rápidas de sierras y múltiples huecos seguidos.

current_level = "level_03"
next_level    = "nitro_menu"   -- vuelve al menú al ganar el último nivel
zone_points   = 100

scene = {
    sprites = {
        [0] = { assetId = "car_yellow_sprite",
                filePath = "./assets/images/car_yellow_1.png" },
    },

    color_textures = {
        [0]  = { assetId="road_factory",  r=30,  g=30,  b=35,  a=255 },
        [1]  = { assetId="wall_metal",    r=50,  g=50,  b=55,  a=255 },
        [2]  = { assetId="player_car",    r=50,  g=130, b=220, a=255 },
        [3]  = { assetId="saw_red",       r=220, g=40,  b=40,  a=255 },
        [4]  = { assetId="saw_orange",    r=255, g=140, b=0,   a=255 },
        [5]  = { assetId="gap_void",      r=5,   g=5,   b=10,  a=255 },
        [6]  = { assetId="ramp_yellow",   r=255, g=210, b=0,   a=255 },
        [7]  = { assetId="checkpoint",    r=240, g=240, b=240, a=255 },
        [8]  = { assetId="hud_bar",       r=0,   g=0,   b=0,   a=180 },
        [9]  = { assetId="dark_overlay",  r=0,   g=0,   b=0,   a=120 },
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
    buttons = {},

    entities = {

        -- ── ROAD ─────────────────────────────────────────────────────────
        [0] = {
            components = {
                transform = { position={x=280,y=0},   scale={x=1,y=1}, rotation=0 },
                sprite    = { asset_id="road_factory", width=240, height=8000, src_rect={x=0,y=0} },
            }
        },

        -- ── PAREDES ──────────────────────────────────────────────────────
        [1] = {
            components = {
                transform    = { position={x=130,y=0}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="wall_metal", width=150, height=8000, src_rect={x=0,y=0} },
                box_collider = { width=150, height=8000 },
                tag          = { tag="wall" },
            }
        },
        [2] = {
            components = {
                transform    = { position={x=520,y=0}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="wall_metal", width=150, height=8000, src_rect={x=0,y=0} },
                box_collider = { width=150, height=8000 },
                tag          = { tag="wall" },
            }
        },

        -- ── Oscuridad ambiental (overlay semitransparente sobre el mundo) ─
        [3] = {
            components = {
                transform = { position={x=0,y=0}, scale={x=1,y=1}, rotation=0 },
                sprite    = { asset_id="dark_overlay", width=800, height=600,
                              src_rect={x=0,y=0}, is_fixed=true },
            }
        },

        -- ══════════ OBSTÁCULOS (de abajo hacia arriba) ═══════════════════

        -- Sierras de pared  y=7200
        [4] = {
            components = {
                transform      = { position={x=280,y=7180}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=80, height=40, src_rect={x=0,y=0} },
                circle_collider= { radius=20, width=40, height=40 },
                tag            = { tag="saw" },
            }
        },
        [5] = {
            components = {
                transform      = { position={x=440,y=7180}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=80, height=40, src_rect={x=0,y=0} },
                circle_collider= { radius=20, width=40, height=40 },
                tag            = { tag="saw" },
            }
        },
        [6] = {
            components = {
                transform = { position={x=280,y=7010}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=240, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- Sierra central giratoria  y=6600
        [7] = {
            components = {
                transform      = { position={x=375,y=6575}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_orange", width=70, height=70, src_rect={x=0,y=0} },
                circle_collider= { radius=35, width=70, height=70 },
                tag            = { tag="saw" },
                script         = { path="./assets/scripts/saw_spin.lua" },
            }
        },
        [8] = {
            components = {
                transform = { position={x=280,y=6420}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=240, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- Rampa 1 / Hueco 1  y=6200 / y=5900–5700
        [9] = {
            components = {
                transform    = { position={x=280,y=6180}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="ramp_yellow", width=240, height=50, src_rect={x=0,y=0} },
                box_collider = { width=240, height=50 },
                tag          = { tag="ramp" },
            }
        },
        [10] = {
            components = {
                transform    = { position={x=280,y=5700}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="gap_void", width=240, height=200, src_rect={x=0,y=0} },
                box_collider = { width=240, height=200 },
                tag          = { tag="gap" },
            }
        },

        -- Doble sierra  y=5300
        [11] = {
            components = {
                transform      = { position={x=285,y=5280}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=50, height=50, src_rect={x=0,y=0} },
                circle_collider= { radius=25, width=50, height=50 },
                tag            = { tag="saw" },
            }
        },
        [12] = {
            components = {
                transform      = { position={x=455,y=5280}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=50, height=50, src_rect={x=0,y=0} },
                circle_collider= { radius=25, width=50, height=50 },
                tag            = { tag="saw" },
            }
        },
        [13] = {
            components = {
                transform = { position={x=280,y=5110}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=240, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- Triple: Rampa / Hueco / Rampa / Hueco (secuencia)  y=4800–4000
        [14] = {
            components = {
                transform    = { position={x=280,y=4780}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="ramp_yellow", width=240, height=50, src_rect={x=0,y=0} },
                box_collider = { width=240, height=50 },
                tag          = { tag="ramp" },
            }
        },
        [15] = {
            components = {
                transform    = { position={x=280,y=4500}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="gap_void", width=240, height=180, src_rect={x=0,y=0} },
                box_collider = { width=240, height=180 },
                tag          = { tag="gap" },
            }
        },
        [16] = {
            components = {
                transform    = { position={x=280,y=4280}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="ramp_yellow", width=240, height=50, src_rect={x=0,y=0} },
                box_collider = { width=240, height=50 },
                tag          = { tag="ramp" },
            }
        },
        [17] = {
            components = {
                transform    = { position={x=280,y=4020}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="gap_void", width=240, height=180, src_rect={x=0,y=0} },
                box_collider = { width=240, height=180 },
                tag          = { tag="gap" },
            }
        },

        -- Sierra giratoria  y=3600
        [18] = {
            components = {
                transform      = { position={x=375,y=3575}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_orange", width=80, height=80, src_rect={x=0,y=0} },
                circle_collider= { radius=40, width=80, height=80 },
                tag            = { tag="saw" },
                script         = { path="./assets/scripts/saw_spin.lua" },
            }
        },
        [19] = {
            components = {
                transform = { position={x=280,y=3400}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=240, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- Doble  y=3000
        [20] = {
            components = {
                transform      = { position={x=285,y=2980}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=50, height=50, src_rect={x=0,y=0} },
                circle_collider= { radius=25, width=50, height=50 },
                tag            = { tag="saw" },
            }
        },
        [21] = {
            components = {
                transform      = { position={x=455,y=2980}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=50, height=50, src_rect={x=0,y=0} },
                circle_collider= { radius=25, width=50, height=50 },
                tag            = { tag="saw" },
            }
        },
        [22] = {
            components = {
                transform = { position={x=280,y=2810}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=240, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- Rampa / Hueco final  y=2500–2000
        [23] = {
            components = {
                transform    = { position={x=280,y=2480}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="ramp_yellow", width=240, height=50, src_rect={x=0,y=0} },
                box_collider = { width=240, height=50 },
                tag          = { tag="ramp" },
            }
        },
        [24] = {
            components = {
                transform    = { position={x=280,y=2200}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="gap_void", width=240, height=220, src_rect={x=0,y=0} },
                box_collider = { width=240, height=220 },
                tag          = { tag="gap" },
            }
        },

        -- Secuencia final de sierras  y=1600 / y=1200
        [25] = {
            components = {
                transform      = { position={x=375,y=1575}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_orange", width=70, height=70, src_rect={x=0,y=0} },
                circle_collider= { radius=35, width=70, height=70 },
                tag            = { tag="saw" },
                script         = { path="./assets/scripts/saw_spin.lua" },
            }
        },
        [26] = {
            components = {
                transform      = { position={x=285,y=1175}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=50, height=50, src_rect={x=0,y=0} },
                circle_collider= { radius=25, width=50, height=50 },
                tag            = { tag="saw" },
            }
        },
        [27] = {
            components = {
                transform      = { position={x=455,y=1175}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=50, height=50, src_rect={x=0,y=0} },
                circle_collider= { radius=25, width=50, height=50 },
                tag            = { tag="saw" },
            }
        },
        [28] = {
            components = {
                transform = { position={x=280,y=1000}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=240, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- CHECKPOINT  y=180
        [29] = {
            components = {
                transform    = { position={x=280,y=160}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="checkpoint", width=240, height=40, src_rect={x=0,y=0} },
                box_collider = { width=240, height=40 },
                tag          = { tag="checkpoint" },
            }
        },

        -- ══════════ PLAYER ════════════════════════════════════════════════
        [30] = {
            components = {
                transform    = { position={x=381,y=7800}, scale={x=0.55,y=0.55}, rotation=0 },
                sprite       = { asset_id="car_yellow_sprite", width=71, height=131, src_rect={x=0,y=0} },
                rigid_body   = { velocity={x=0,y=0} },
                box_collider = { width=71, height=131 },
                circle_collider={ radius=35, width=71, height=131 },
                tag          = { tag="player" },
                player       = { base_speed=330, rotation_speed=150, jump_duration=1.7,
                              jump_max_scale=1.7, base_scale_x=0.55, base_scale_y=0.55 },
                nitro        = { cooldown=1.0, boost_duration=1.0 },
                script       = { path="./assets/scripts/player_car.lua" },
            }
        },

        -- ══════════ HUD ═══════════════════════════════════════════════════
        [31] = {
            components = {
                transform = { position={x=0,y=0}, scale={x=1,y=1}, rotation=0 },
                sprite    = { asset_id="hud_bar", width=800, height=40,
                              src_rect={x=0,y=0}, is_fixed=true },
            }
        },
        [32] = {
            components = {
                transform = { position={x=10,y=8}, scale={x=1,y=1}, rotation=0 },
                text = { text="SCORE: 0", font_asset_id="font_hud",
                         color={r=255,g=255,b=80,a=255}, is_fixed=true },
                script = { path="./assets/scripts/hud_score.lua" },
            }
        },
        [33] = {
            components = {
                transform = { position={x=460,y=8}, scale={x=1,y=1}, rotation=0 },
                text = { text="NITRO: READY [SPC]", font_asset_id="font_hud",
                         color={r=80,g=255,b=80,a=255}, is_fixed=true },
                script = { path="./assets/scripts/hud_nitro.lua" },
            }
        },
        [34] = {
            components = {
                transform = { position={x=280,y=50}, scale={x=1,y=1}, rotation=0 },
                text = { text="LEVEL 3 - FACTORY", font_asset_id="font_hud2",
                         color={r=255,g=140,b=80,a=200}, is_fixed=true },
            }
        },
    },
}

zone_points = 100
