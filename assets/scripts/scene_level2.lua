-- scene_level2.lua  —  Nitro Rush: Level 2 — Mountain / Dirt
-- Dificultad media. Camino más angosto (260px), más huecos y curvas.
--
-- El camino se desplaza lateralmente en tres tramos para simular curvas:
--   Tramo A (y=7000–5000): carretera centrada en x=270 (camino estrecho izq)
--   Tramo B (y=5000–3000): carretera centrada en x=310 (giro suave derecha)
--   Tramo C (y=3000–0   ): carretera centrada en x=270 (vuelta al centro)
-- Las paredes acompañan cada desplazamiento.

current_level = "level_02"
next_level    = "level_03"
zone_points   = 100

scene = {
    sprites = {
        [0] = { assetId = "car_yellow_sprite",
                filePath = "./assets/images/car_yellow_1.png" },
    },

    color_textures = {
        [0]  = { assetId="road_dirt",     r=90,  g=65,  b=40,  a=255 },
        [1]  = { assetId="wall_rock",     r=55,  g=40,  b=25,  a=255 },
        [2]  = { assetId="player_car",    r=50,  g=130, b=220, a=255 },
        [3]  = { assetId="saw_red",       r=220, g=40,  b=40,  a=255 },
        [4]  = { assetId="gap_void",      r=5,   g=5,   b=10,  a=255 },
        [5]  = { assetId="ramp_yellow",   r=255, g=210, b=0,   a=255 },
        [6]  = { assetId="checkpoint",    r=240, g=240, b=240, a=255 },
        [7]  = { assetId="hud_bar",       r=0,   g=0,   b=0,   a=180 },
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

        -- ── ROAD (tres tramos) ─────────────────────────────────────────────
        [0] = {
            components = {
                transform = { position={x=270,y=5000}, scale={x=1,y=1}, rotation=0 },
                sprite    = { asset_id="road_dirt", width=260, height=2000, src_rect={x=0,y=0} },
            }
        },
        [1] = {
            components = {
                transform = { position={x=310,y=3000}, scale={x=1,y=1}, rotation=0 },
                sprite    = { asset_id="road_dirt", width=260, height=2000, src_rect={x=0,y=0} },
            }
        },
        [2] = {
            components = {
                transform = { position={x=270,y=0},   scale={x=1,y=1}, rotation=0 },
                sprite    = { asset_id="road_dirt", width=260, height=3000, src_rect={x=0,y=0} },
            }
        },

        -- ── PAREDES tramo A (y=5000–7000) ─────────────────────────────────
        [3] = { -- izquierda
            components = {
                transform    = { position={x=120,y=5000}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="wall_rock", width=150, height=2000, src_rect={x=0,y=0} },
                box_collider = { width=150, height=2000 },
                tag          = { tag="wall" },
            }
        },
        [4] = { -- derecha
            components = {
                transform    = { position={x=530,y=5000}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="wall_rock", width=150, height=2000, src_rect={x=0,y=0} },
                box_collider = { width=150, height=2000 },
                tag          = { tag="wall" },
            }
        },

        -- ── PAREDES tramo B (y=3000–5000) ─────────────────────────────────
        [5] = { -- izquierda (desplazada 40px a la derecha)
            components = {
                transform    = { position={x=160,y=3000}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="wall_rock", width=150, height=2000, src_rect={x=0,y=0} },
                box_collider = { width=150, height=2000 },
                tag          = { tag="wall" },
            }
        },
        [6] = { -- derecha
            components = {
                transform    = { position={x=570,y=3000}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="wall_rock", width=150, height=2000, src_rect={x=0,y=0} },
                box_collider = { width=150, height=2000 },
                tag          = { tag="wall" },
            }
        },

        -- ── PAREDES tramo C (y=0–3000) ─────────────────────────────────────
        [7] = {
            components = {
                transform    = { position={x=120,y=0}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="wall_rock", width=150, height=3000, src_rect={x=0,y=0} },
                box_collider = { width=150, height=3000 },
                tag          = { tag="wall" },
            }
        },
        [8] = {
            components = {
                transform    = { position={x=530,y=0}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="wall_rock", width=150, height=3000, src_rect={x=0,y=0} },
                box_collider = { width=150, height=3000 },
                tag          = { tag="wall" },
            }
        },

        -- ══════════ OBSTÁCULOS ══════════════════════════════════════════════

        -- Sierra de pared izquierda  y=6400
        [9] = {
            components = {
                transform      = { position={x=270,y=6380}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=90, height=40, src_rect={x=0,y=0} },
                circle_collider= { radius=20, width=40, height=40 },
                tag            = { tag="saw" },
            }
        },
        [10] = {
            components = {
                transform = { position={x=270,y=6230}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=260, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- Sierra central  y=5700
        [11] = {
            components = {
                transform      = { position={x=375,y=5680}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=60, height=60, src_rect={x=0,y=0} },
                circle_collider= { radius=30, width=60, height=60 },
                tag            = { tag="saw" },
            }
        },
        [12] = {
            components = {
                transform = { position={x=270,y=5530}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=260, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- Rampa 1  y=5100
        [13] = {
            components = {
                transform    = { position={x=270,y=5080}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="ramp_yellow", width=260, height=50, src_rect={x=0,y=0} },
                box_collider = { width=260, height=50 },
                tag          = { tag="ramp" },
            }
        },
        -- Hueco 1  y=4800–4600
        [14] = {
            components = {
                transform    = { position={x=270,y=4600}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="gap_void", width=260, height=200, src_rect={x=0,y=0} },
                box_collider = { width=260, height=200 },
                tag          = { tag="gap" },
            }
        },

        -- Sierra doble  y=4000
        [15] = {
            components = {
                transform      = { position={x=275,y=3980}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=50, height=50, src_rect={x=0,y=0} },
                circle_collider= { radius=25, width=50, height=50 },
                tag            = { tag="saw" },
            }
        },
        [16] = {
            components = {
                transform      = { position={x=465,y=3980}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=50, height=50, src_rect={x=0,y=0} },
                circle_collider= { radius=25, width=50, height=50 },
                tag            = { tag="saw" },
            }
        },
        [17] = {
            components = {
                transform = { position={x=310,y=3830}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=260, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- Rampa 2  y=3300
        [18] = {
            components = {
                transform    = { position={x=310,y=3280}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="ramp_yellow", width=260, height=50, src_rect={x=0,y=0} },
                box_collider = { width=260, height=50 },
                tag          = { tag="ramp" },
            }
        },
        -- Hueco 2  y=3000–2800
        [19] = {
            components = {
                transform    = { position={x=310,y=2800}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="gap_void", width=260, height=200, src_rect={x=0,y=0} },
                box_collider = { width=260, height=200 },
                tag          = { tag="gap" },
            }
        },

        -- Sierra giratoria  y=2200
        [20] = {
            components = {
                transform      = { position={x=370,y=2180}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=70, height=70, src_rect={x=0,y=0} },
                circle_collider= { radius=35, width=70, height=70 },
                tag            = { tag="saw" },
                script         = { path="./assets/scripts/saw_spin.lua" },
            }
        },
        [21] = {
            components = {
                transform = { position={x=270,y=2020}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=260, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- Rampa 3  y=1500
        [22] = {
            components = {
                transform    = { position={x=270,y=1480}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="ramp_yellow", width=260, height=50, src_rect={x=0,y=0} },
                box_collider = { width=260, height=50 },
                tag          = { tag="ramp" },
            }
        },
        -- Hueco 3  y=1200–1000
        [23] = {
            components = {
                transform    = { position={x=270,y=1000}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="gap_void", width=260, height=200, src_rect={x=0,y=0} },
                box_collider = { width=260, height=200 },
                tag          = { tag="gap" },
            }
        },

        -- Sierra doble final  y=600
        [24] = {
            components = {
                transform      = { position={x=275,y=580}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=50, height=50, src_rect={x=0,y=0} },
                circle_collider= { radius=25, width=50, height=50 },
                tag            = { tag="saw" },
            }
        },
        [25] = {
            components = {
                transform      = { position={x=465,y=580}, scale={x=1,y=1}, rotation=0 },
                sprite         = { asset_id="saw_red", width=50, height=50, src_rect={x=0,y=0} },
                circle_collider= { radius=25, width=50, height=50 },
                tag            = { tag="saw" },
            }
        },
        [26] = {
            components = {
                transform = { position={x=270,y=430}, scale={x=1,y=1}, rotation=0 },
                box_collider={ width=260, height=20 },
                tag       = { tag="score_zone" },
                script    = { path="./assets/scripts/score_zone.lua" },
            }
        },

        -- CHECKPOINT  y=180
        [27] = {
            components = {
                transform    = { position={x=270,y=160}, scale={x=1,y=1}, rotation=0 },
                sprite       = { asset_id="checkpoint", width=260, height=40, src_rect={x=0,y=0} },
                box_collider = { width=260, height=40 },
                tag          = { tag="checkpoint" },
            }
        },

        -- ══════════════ PLAYER ════════════════════════════════════════════
        [28] = {
            components = {
                transform    = { position={x=381,y=6800}, scale={x=0.55,y=0.55}, rotation=0 },
                sprite       = { asset_id="car_yellow_sprite", width=71, height=131, src_rect={x=0,y=0} },
                rigid_body   = { velocity={x=0,y=0} },
                box_collider = { width=71, height=131 },
                circle_collider={ radius=35, width=71, height=131 },
                tag          = { tag="player" },
                player       = { base_speed=310, rotation_speed=140, jump_duration=1.7,
                              jump_max_scale=1.7, base_scale_x=0.55, base_scale_y=0.55 },
                nitro        = { cooldown=1.0, boost_duration=1.0 },
                script       = { path="./assets/scripts/player_car.lua" },
            }
        },

        -- ══════════════ HUD ═══════════════════════════════════════════════
        [29] = {
            components = {
                transform = { position={x=0,y=0}, scale={x=1,y=1}, rotation=0 },
                sprite    = { asset_id="hud_bar", width=800, height=40,
                              src_rect={x=0,y=0}, is_fixed=true },
            }
        },
        [30] = {
            components = {
                transform = { position={x=10,y=8}, scale={x=1,y=1}, rotation=0 },
                text = { text="SCORE: 0", font_asset_id="font_hud",
                         color={r=255,g=255,b=80,a=255}, is_fixed=true },
                script = { path="./assets/scripts/hud_score.lua" },
            }
        },
        [31] = {
            components = {
                transform = { position={x=460,y=8}, scale={x=1,y=1}, rotation=0 },
                text = { text="NITRO: READY [SPC]", font_asset_id="font_hud",
                         color={r=80,g=255,b=80,a=255}, is_fixed=true },
                script = { path="./assets/scripts/hud_nitro.lua" },
            }
        },
        [32] = {
            components = {
                transform = { position={x=290,y=50}, scale={x=1,y=1}, rotation=0 },
                text = { text="LEVEL 2 - MOUNTAIN", font_asset_id="font_hud2",
                         color={r=200,g=160,b=100,a=200}, is_fixed=true },
            }
        },
    },
}

zone_points = 100
